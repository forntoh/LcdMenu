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

extern MENU_ITEM mainMenu[];
extern MENU_ITEM subMenu[];

MENU_ITEM subMenu[] = {{MENU_ITEM_TYPE_SUB_MENU_HEADER, "", NULL, mainMenu},
                       {MENU_ITEM_TYPE_COMMAND, "Sub slow", NULL, NULL},
                       {MENU_ITEM_TYPE_COMMAND, "Sub fast", NULL, NULL},
                       {MENU_ITEM_TYPE_COMMAND, "Sub heart", NULL, NULL},
                       {MENU_ITEM_TYPE_COMMAND, "Sub SOS", NULL, NULL},
                       {MENU_ITEM_TYPE_COMMAND, "Sub random", NULL, NULL},
                       {MENU_ITEM_TYPE_END_OF_MENU, "", NULL, NULL}};

MENU_ITEM mainMenu[] = {{MENU_ITEM_TYPE_MAIN_MENU_HEADER, "", NULL, mainMenu},
                        {MENU_ITEM_TYPE_SUB_MENU, "Blink slow", NULL, NULL},
                        {MENU_ITEM_TYPE_COMMAND, "Blink fast", NULL, NULL},
                        {MENU_ITEM_TYPE_SUB_MENU, "Blink heart", NULL, subMenu},
                        {MENU_ITEM_TYPE_COMMAND, "Blink SOS", NULL, NULL},
                        {MENU_ITEM_TYPE_COMMAND, "Blink random", NULL, NULL},
                        {MENU_ITEM_TYPE_END_OF_MENU, "", NULL, NULL}};

LcdMenu menu1(LCD_ROWS, LCD_COLS);

Keypad keypad =
    Keypad(makeKeymap(keys), rowPins, colPins, KEYPAD_ROWS, KEYPAD_COLS);

void setup() { menu1.setupLcdWithMenu(LCD_ADDR, mainMenu); }

void loop() {
    char key = keypad.getKey();
    if (key == NO_KEY) return;

    switch (key) {
        case 'A':
            menu1.up();
            break;
        case 'B':
            menu1.down();
            break;
        case 'C':
            menu1.select();
            break;
        case 'D':
            menu1.back();
            break;
        default:
            break;
    }
}
