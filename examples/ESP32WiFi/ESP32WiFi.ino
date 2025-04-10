#include <ItemCommand.h>
#include <ItemInput.h>
#include <ItemInputCharset.h>
#include <ItemSubMenu.h>
#include <LcdMenu.h>
#include <MenuScreen.h>
#include <WiFi.h>  // ESP32 WiFi library
#include <display/LiquidCrystal_I2CAdapter.h>
#include <input/KeyboardAdapter.h>
#include <renderer/CharacterDisplayRenderer.h>

#define LCD_ROWS 2
#define LCD_COLS 16

// Define the character set for input fields
const char* charset = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

// Declare the main menu and WiFi menu
extern MenuScreen* wifiMenu;

// Define the main menu structure
MENU_SCREEN(
    mainMenu,
    mainItems,
    ITEM_BASIC("Start service"),              // Option to start a service
    ITEM_SUBMENU("WiFi Networks", wifiMenu),  // Submenu for WiFi networks
    ITEM_BASIC("Settings"),                   // Option for settings
    ITEM_BASIC("Exit"));                      // Option to exit

// Define the WiFi menu structure
MENU_SCREEN(wifiMenu, wifiItems, ITEM_BASIC(""));

LiquidCrystal_I2C lcd(0x27, LCD_COLS, LCD_ROWS);                     // Initialize the LCD display
LiquidCrystal_I2CAdapter lcdAdapter(&lcd);                           // Adapter for the LCD
CharacterDisplayRenderer renderer(&lcdAdapter, LCD_COLS, LCD_ROWS);  // Renderer for the LCD
LcdMenu menu(renderer);                                              // Main menu object
KeyboardAdapter keyboard(&menu, &Serial);                            // Keyboard adapter for menu navigation

unsigned long previousScanMillis = 0;      // Timestamp for the last WiFi scan
const unsigned long scanInterval = 10000;  // Interval between WiFi scans (10 seconds)
bool isScanning = false;                   // Flag to indicate if a scan is in progress

// Start an asynchronous WiFi scan
void startWifiScan() {
    Serial.println("Starting WiFi scan...");
    wifiMenu->clear();                             // Clear the WiFi menu
    wifiMenu->addItem(ITEM_BASIC("Scanning..."));  // Add a placeholder item
    menu.refresh();                                // Refresh the menu display

    int result = WiFi.scanNetworks(true);  // Start asynchronous scan
    if (result == WIFI_SCAN_RUNNING) {
        isScanning = true;  // Mark scanning as in progress
    } else {
        Serial.println("Failed to start scan or scan already in progress!");
    }
}

// Check if the WiFi scan is complete and update the menu
void checkWifiScan() {
    if (isScanning) {
        int networks = WiFi.scanComplete();  // Check scan status
        if (networks == -1) {
            // Scan is still in progress; return immediately
            return;
        }

        Serial.println("WiFi scan complete.");
        wifiMenu->clear();  // Clear the WiFi menu

        if (networks < 0) {
            // No networks found
            Serial.println("No networks found.");
            wifiMenu->addItem(ITEM_BASIC("No networks found"));
            wifiMenu->addItem(ITEM_COMMAND("Retry", startWifiScan));  // Option to retry scanning
        } else {
            // Display the list of found networks
            Serial.printf("%d networks found:\n", networks);
            for (int i = 0; i < networks; i++) {
                String networkName = WiFi.SSID(i);                          // Get network name
                int rssi = WiFi.RSSI(i);                                    // Get signal strength
                bool isSecured = WiFi.encryptionType(i) != WIFI_AUTH_OPEN;  // Check if secured
                String itemName = networkName + " (" + (isSecured ? "Secured" : "Open") + ")";
                char* itemText = strdup(itemName.c_str());  // Copy network name for menu item

                // Create a submenu for secured networks
                static MenuScreen* securedNetworkMenu = new MenuScreen();  // Submenu for secured networks
                securedNetworkMenu->setParent(wifiMenu);                   // Set parent menu
                securedNetworkMenu->clear();                               // Clear the submenu

                char* password = new char[32];  // Buffer for the password

                // Add an input field for the password
                ItemInput* passwordItem = ITEM_INPUT_CHARSET("Pwd", charset, [password, itemName](const char* value) {
                    strncpy(password, value, 32);  // Store the entered password
                    password[31] = '\0';           // Ensure null termination
                    Serial.printf("Password set for %s: %s\n", itemName.c_str(), password);
                });
                securedNetworkMenu->addItem(passwordItem);

                // Add a command to connect to the network
                securedNetworkMenu->addItem(ITEM_COMMAND("Connect", [networkName, password, isSecured, passwordItem]() {
                    MenuItem* connectItem = securedNetworkMenu->getItemAt(1);  // Get the "Connect" item

                    // Replace "Connect" with "Connecting..."
                    securedNetworkMenu->removeLastItem();
                    securedNetworkMenu->addItem(ITEM_BASIC("Connecting..."));
                    menu.refresh();

                    if (isSecured) {
                        // Connect to secured network
                        Serial.printf("Connecting to %s with password %s...\n", networkName.c_str(), password);
                        WiFi.begin(networkName.c_str(), password);
                    } else {
                        // Connect to open network
                        Serial.printf("Connecting to %s...\n", networkName.c_str());
                        WiFi.begin(networkName.c_str());
                    }

                    unsigned long startAttemptTime = millis();
                    while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 10000) {
                        delay(500);  // Wait for connection
                        Serial.print(".");
                    }

                    if (WiFi.status() == WL_CONNECTED) {
                        // Successfully connected
                        Serial.printf("\nConnected to %s. IP: %s\n", networkName.c_str(), WiFi.localIP().toString().c_str());
                        passwordItem->setValue((char*)"********");  // Hide the password
                        securedNetworkMenu->removeLastItem();
                        securedNetworkMenu->addItem(ITEM_COMMAND("Connected", [connectItem, passwordItem]() {
                            Serial.println("Disconnecting...");

                            // Replace "Connected" with "Disconnecting..."
                            securedNetworkMenu->removeLastItem();
                            securedNetworkMenu->addItem(ITEM_BASIC("Disconnecting..."));
                            menu.refresh();

                            WiFi.disconnect();  // Disconnect from the network

                            unsigned long startAttemptTime = millis();
                            while (WiFi.status() == WL_CONNECTED && millis() - startAttemptTime < 10000) {
                                delay(500);  // Wait for disconnection
                                Serial.print(".");
                            }

                            Serial.println("\nDisconnected.");

                            // Restore the "Connect" item
                            securedNetworkMenu->removeLastItem();
                            securedNetworkMenu->addItem(connectItem);

                            // Clear the password
                            passwordItem->setValue((char*)"");
                            menu.refresh();
                        }));
                    } else {
                        // Connection failed
                        Serial.printf("\nFailed to connect to %s.\n", networkName.c_str());
                        securedNetworkMenu->removeLastItem();
                        securedNetworkMenu->addItem(connectItem);  // Restore the "Connect" item
                    }

                    menu.refresh();
                }));

                // Add the submenu to the main WiFi menu
                wifiMenu->addItem(ITEM_SUBMENU(itemText, securedNetworkMenu));

                Serial.printf("%d: %s (%d dBm)\n", i + 1, networkName.c_str(), rssi);
            }
        }

        // Free the memory used for scan results
        WiFi.scanDelete();
        isScanning = false;  // Mark scanning as complete
        menu.refresh();
    }
}

void setup() {
    Serial.begin(115200);      // Initialize serial communication
    WiFi.mode(WIFI_STA);       // Set WiFi to station mode
    renderer.begin();          // Initialize the LCD renderer
    menu.setScreen(mainMenu);  // Set the main menu as the active screen
    startWifiScan();           // Start the first WiFi scan
}

void loop() {
    keyboard.observe();  // Observe keyboard input for menu navigation

    // Periodically check the WiFi scan results if the user is in the networks submenu
    if (menu.getScreen() == wifiMenu) {
        unsigned long currentMillis = millis();
        if (currentMillis - previousScanMillis >= scanInterval) {
            previousScanMillis = currentMillis;
            startWifiScan();  // Start a new WiFi scan
        }
    }
    checkWifiScan();  // Check the status of the WiFi scan
}