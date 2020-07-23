/*
 Dynamic Menu

 This sketch demostrates how to dynamically create sub menu's during runtime
 using the LcdMenu library.

 Circuit:
 * Arduino Board
 * Keypad pin 1 to digital pin 9
 * Keypad pin 2 to digital pin 8
 * Keypad pin 3 to digital pin 7
 * Keypad pin 4 to digital pin 6
 * Keypad pin 5 to digital pin 5
 * Keypad pin 6 to digital pin 4
 * Keypad pin 7 to digital pin 3
 * Keypad pin 8 to digital pin 2
 * LCD SLC pin to arduino SLC pin
 * LCD SDA pin to arduino SDA pin

 created 23 July 2020
 by Forntoh Thomas

 This example is in the public domain.

 https://github.com/forntoh/LcdMenu/tree/master/examples/DynamicMenu/DynamicMenu.ino

*/

#include <Keypad.h>
#include <LcdMenu.h>

#define LCD_ROWS 2
#define LCD_COLS 16

// Configure keypad keys
char keys[4][4] = {{'1', '2', '3', 'A'},
                   {'4', '5', '6', 'B'},
                   {'7', '8', '9', 'C'},
                   {'*', '0', '#', 'D'}};

// Configure keypad pins
byte colPins[4] = {5, 4, 3, 2};
byte rowPins[4] = {9, 8, 7, 6};

extern MenuItem mainMenu[];

MenuItem mainMenu[] = {ItemHeader (                       ),
                       MenuItem   ("Start service  "      ),
                       ItemSubMenu("Connect to WiFi", NULL),
                       MenuItem   ("Settings       "      ),
                       ItemFooter (                       )};

char* names[] = {"TP-LINK_AP_F558", "iH2K-7539", "KTA-CONNECT", "SM-G955U241"};

LcdMenu menu(LCD_ROWS, LCD_COLS);

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, 4, 4);

void setup() { menu.setupLcdWithMenu(0x27, mainMenu); }

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
            menu.enter();
            break;
        case 'D':
            menu.back();
            break;
        case '*':
            createSubMenu();
        default:
            break;
    }
}

void createSubMenu() {
    // Create the items
    MenuItem myItems[4];

    // Loop through list and add each item to the menu
    for (uint8_t i = 0; i < 4; i++) {
        // Create MenuItem to be added
        MenuItem wifiParamsMenu[] = {MenuItem(names[i]),
                                     ItemInput("Pass", "", NULL)};
        // Insert MenuItem into array
        myItems[i] =
            ItemSubMenu(names[i], menu.buildSubMenu(wifiParamsMenu, 2));
    }

    /** Set the submenu, (precise the position on the display and the items to
     * use). Use the buildSubMenu function to generate a submenu following
     * LcdMenu's style
     */
    menu.setSubMenu(1, menu.buildSubMenu(myItems, 4));
}