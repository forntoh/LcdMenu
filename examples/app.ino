#include <Key.h>
#include <Keypad.h>

#include "LcdMenu.h"

#define LCD_ADDR 0x27

#define LCD_ROWS 4
#define LCD_COLS 20

#define KEYPAD_ROWS 4
#define KEYPAD_COLS 4

char keys[KEYPAD_ROWS][KEYPAD_COLS] = {{'1', '2', '3', 'A'},
                                       {'4', '5', '6', 'B'},
                                       {'7', '8', '9', 'C'},
                                       {'*', '0', '#', 'D'}};

byte colPins[KEYPAD_ROWS] = {5, 4, 3, 2};
byte rowPins[KEYPAD_COLS] = {9, 8, 7, 6};

extern MenuItem mainMenu[];
extern MenuItem wifiItemsMenu[];
extern MenuItem wifiParamsMenu[];
extern MenuItem settingsMenu[];

String password = "";

MenuItem mainMenu[] = {ItemHeader(),
                       MenuItem("Start service"),
                       ItemSubMenu("Connect to WiFi", wifiItemsMenu),
                       ItemSubMenu("Settings", settingsMenu),
                       MenuItem("Blink SOS"),
                       MenuItem("Blink random"),
                       ItemFooter()};

MenuItem wifiItemsMenu[] = {ItemSubHeader(mainMenu),
                            ItemSubMenu("TP-LINK_AP_F558", wifiParamsMenu),
                            ItemSubMenu("iH2K-7539", wifiParamsMenu),
                            ItemSubMenu("KTA-CONNECT", wifiParamsMenu),
                            ItemSubMenu("SM-G955U241", wifiParamsMenu),
                            ItemSubMenu("DIRECT-dc-SM-G950N", wifiParamsMenu),
                            ItemFooter()};

MenuItem wifiParamsMenu[] = {ItemSubHeader(wifiItemsMenu),
                             MenuItem("TP-LINK_AP_F558"),
                             ItemInput("Pass", "", NULL), ItemFooter()};

MenuItem settingsMenu[] = {ItemSubHeader(mainMenu),
                           ItemToggle("Backlight", toggleBacklight),
                           MenuItem("Contrast"), ItemFooter()};

LcdMenu menu(LCD_ROWS, LCD_COLS);

Keypad keypad =
    Keypad(makeKeymap(keys), rowPins, colPins, KEYPAD_ROWS, KEYPAD_COLS);

void setup() { menu.setupLcdWithMenu(LCD_ADDR, mainMenu); }

void loop() {
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
            menu.select();
            break;
        case 'D':
            password = "";
            menu.back();
            break;
        case '*':
            password = password.substring(0, password.length() - 1);
            menu.setText(password);
            break;
        default:
            password += key;
            menu.setText(password);
            break;
    }
}

void toggleBacklight() { menu.lcd->setBacklight(settingsMenu[1].isOn); }