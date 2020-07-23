#include <Keypad.h>

#include <LcdMenu.h>

#define LCD_ADDR 0x27

#define LCD_ROWS 2
#define LCD_COLS 16

#define KEYPAD_ROWS 4
#define KEYPAD_COLS 4

void toggleBacklight();

char keys[KEYPAD_ROWS][KEYPAD_COLS] = {{'1', '2', '3', 'A'},
                                       {'4', '5', '6', 'B'},
                                       {'7', '8', '9', 'C'},
                                       {'*', '0', '#', 'D'}};

byte colPins[KEYPAD_ROWS] = {5, 4, 3, 2};
byte rowPins[KEYPAD_COLS] = {9, 8, 7, 6};

extern MenuItem mainMenu[];
extern MenuItem settingsMenu[];

String password = "";

MenuItem mainMenu[] = {ItemHeader(),
                       MenuItem("Start service"),
                       ItemSubMenu("Connect to WiFi", NULL),
                       ItemSubMenu("Settings", settingsMenu),
                       MenuItem("Blink SOS"),
                       MenuItem("Blink random"),
                       ItemFooter()};

MenuItem settingsMenu[] = {ItemSubHeader(mainMenu),
                           ItemToggle("Backlight", toggleBacklight),
                           MenuItem("Contrast"), ItemFooter()};

char* names[] = {"TP-LINK_AP_F558", "iH2K-7539", "KTA-CONNECT", "SM-G955U241"};

LcdMenu menu(LCD_ROWS, LCD_COLS);

Keypad keypad =
    Keypad(makeKeymap(keys), rowPins, colPins, KEYPAD_ROWS, KEYPAD_COLS);

void setup() {
    Serial.begin(115200);
    menu.setupLcdWithMenu(LCD_ADDR, mainMenu);

    MenuItem myItems[4];
    for (uint8_t i = 0; i < 4; i++) {
        MenuItem wifiParamsMenu[] = {MenuItem(names[i]),
                                     ItemInput("Pass", "", NULL)};

        myItems[i] =
            ItemSubMenu(names[i], menu.buildSubMenu(wifiParamsMenu, 2));
    }
    menu.setSubMenu(1, menu.buildSubMenu(myItems, 4));

    delay(3500);
    menu.displayNotification("Helo", 25000);
}

void loop() {
    menu.updateTimer();

    char key = keypad.getKey();
    if (key == NO_KEY) return;

    switch (key) {
        case 'A':
            menu.up();
            break;
        case 'B':
            menu.down();
            break;
        case 'C':
            menu.enter();
            break;
        case 'D':
            password = "";
            menu.back();
            break;
        case '*':
            password = password.substring(0, password.length() - 1);
            menu.setText(password);
            break;
        case '#':
            break;
        default:
            password += key;
            menu.setText(password);
            break;
    }
}

void toggleBacklight() { menu.lcd->setBacklight(settingsMenu[1].isOn); }