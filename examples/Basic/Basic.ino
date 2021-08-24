/*
 Basic Menu Navigation

 This sketch demostrates how to get started with the LcdMenu library

 Circuit:
 * Arduino Board
 * LCD SLC pin to arduino SLC pin
 * LCD SDA pin to arduino SDA pin

 created 22 July 2020
 by Forntoh Thomas

 This example is in the public domain.

 https://github.com/forntoh/LcdMenu/tree/master/examples/Basic/Basic.ino

*/

#include <LcdMenu.h>

#define LCD_ROWS 2
#define LCD_COLS 16

// Configure keyboard keys (ASCII)
#define UP 56        // NUMPAD 8
#define DOWN 50      // NUMPAD 2
#define LEFT 52      // NUMPAD 4
#define RIGHT 54     // NUMPAD 6
#define ENTER 53     // NUMPAD 5
#define BACK 55      // NUMPAD 7
#define BACKSPACE 8  // BACKSPACE
#define CLEAR 46     // NUMPAD .

// Define the main menu
extern MenuItem mainMenu[];

// Initialize the main menu items
MenuItem mainMenu[] = {ItemHeader(),
                       MenuItem("Start service"),
                       MenuItem("Connect to WiFi"),
                       MenuItem("Settings"),
                       MenuItem("Blink SOS"),
                       MenuItem("Blink random"),
                       ItemFooter()};
// Construct the LcdMenu
LcdMenu menu(LCD_ROWS, LCD_COLS);

void setup() {
    Serial.begin(9600);
    // Initialize LcdMenu with the menu items
    menu.setupLcdWithMenu(0x27, mainMenu);
}

void loop() {
    if (!Serial.available()) return;
    char command = Serial.read();

    if (command == UP)
        menu.up();
    else if (command == DOWN)
        menu.down();
    else if (command == LEFT)
        menu.left();
    else if (command == RIGHT)
        menu.right();
    else if (command == ENTER)
        menu.enter();
    else if (command == BACK)
        menu.back();
    else if (command == CLEAR)
        menu.clear();
    else if (command == BACKSPACE)
        menu.backspace();
    else
        menu.type((String)command);
}