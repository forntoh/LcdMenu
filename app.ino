#include <Key.h>
#include <Keypad.h>

#include "Menus.h"

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

String menuItems[] = {"Item 1", "ITEM 2", "ITEM 3", "ITEM 4",
                      "ITEM 5", "ITEM 6", "ITEM 7", "ITEM 8"};

Menus menu1(LCD_ROWS, LCD_COLS, menuItems);

Keypad keypad =
    Keypad(makeKeymap(keys), rowPins, colPins, KEYPAD_ROWS, KEYPAD_COLS);

void setup() {
    Serial.begin(9600);
    menu1.setupLCD(LCD_ADDR);
    menu1.drawMenu();
    menu1.drawCursor();
}

void loop() {
    char key = keypad.getKey();
    if (key != NO_KEY) {
        switch (key) {
            case 'A':
                menu1.up();
                break;
            case 'B':
                menu1.down();
                break;
            default:
                break;
        }
    }
}
