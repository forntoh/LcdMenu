// Libraries
#include <Arduino_FreeRTOS.h>
#include <ItemSubMenu.h>
#include <ItemToggle.h>
#include <LcdMenu.h>
#include <MenuScreen.h>
#include <display/LiquidCrystal_I2CAdapter.h>
#include <input/KeyboardAdapter.h>

// 2x20 LCD Display
#define LCD_ROWS 4
#define LCD_COLS 20

float temperature1;
float temperature2;
float temperature3;

bool relay1State, relay2State, relay3State;

// Relay 1 Toggle
void toggleRelay1(uint16_t isOn) {
    printLog(F("relay1State"));
}

// Relay 2 Toggle
void toggleRelay2(uint16_t isOn) {
    printLog(F("relay2State"));
}

// Relay 3 Toggle
void toggleRelay3(uint16_t isOn) {
    printLog(F("relay3State"));
}

LiquidCrystal_I2C lcd(0x27, LCD_COLS, LCD_ROWS);
LiquidCrystal_I2CAdapter lcdAdapter(&lcd, LCD_COLS, LCD_ROWS);
LcdMenu menu(lcdAdapter);
KeyboardAdapter keyboard(&menu, &Serial);

extern MenuScreen* relayScreen;
extern MenuScreen* tempScreen;

// clang-format off
MENU_SCREEN(mainScreen, mainItems,
    ITEM_BASIC("Relay Test"),
    ITEM_SUBMENU("Temp. Observing", tempScreen),
    ITEM_BASIC("Hello World"),
    ITEM_BASIC("I'm World"));
// clang-format on

// Sub Menu 1: Relays
// clang-format off
MENU_SCREEN(relayScreen, relayItems, 
    ITEM_TOGGLE("Relay 1", toggleRelay1), 
    ITEM_TOGGLE("Relay 2", toggleRelay2), 
    ITEM_TOGGLE("Relay 3", toggleRelay3));
// clang-format on

// Sub Menu 2: Temperature Values
// clang-format off
MENU_SCREEN(tempScreen, tempItems, 
    ITEM_BASIC(""), 
    ITEM_BASIC(""), 
    ITEM_BASIC(""));
// clang-format on

// RTOS func. to measure temperature value
static void tempMeas(void* pvParameters) {
    (void)pvParameters;
    for (;;) {
        //-------------------BEGIN: TEST WITH RANDOM------------------//
        temperature1 = random(1, 1000) / 100.0;  // Generate random float
        temperature2 = random(1, 2000) / 100.0;  // Generate random float
        temperature3 = random(1, 3000) / 100.0;  // Generate random float
        //-------------------END: TEST WITH RANDOM--------------------//

        char buffer1[8];
        tempItems[0]->setText(dtostrf(temperature1, 5, 2, buffer1));

        char buffer2[8];
        tempItems[1]->setText(dtostrf(temperature2, 5, 2, buffer2));

        char buffer3[8];
        tempItems[2]->setText(dtostrf(temperature3, 5, 2, buffer3));

        if (menu.getScreen() == tempScreen) {
            menu.refresh();
        }
        vTaskDelay(5000 / portTICK_PERIOD_MS);  // wait for five seconds
    }
}

// RTOS func to read serial input from keyboard
static void keyPad(void* pvParameters) {
    (void)pvParameters;  // Unused parameter

    for (;;) {
        keyboard.observe();
        vTaskDelay(1);
    }
}

void setup() {
    Serial.begin(9600);
    // LCD activation
    lcdAdapter.begin();
    menu.setScreen(mainScreen);
    // Run RTOS func.
    xTaskCreate(keyPad, "keyPad", 128, NULL, tskIDLE_PRIORITY + 2, NULL);
    xTaskCreate(tempMeas, "tempMeas", 128, NULL, tskIDLE_PRIORITY + 1, NULL);
    // Setup random seed
    pinMode(A0, INPUT);
    randomSeed(analogRead(A0));
}

void loop() {
    lcdAdapter.updateTimer();
}
