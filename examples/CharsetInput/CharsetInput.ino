/*
 Charset Input

 This sketch demostrates how to use ItemInput in the LcdMenu library with a
 predifined charset

 Circuit:
 * Arduino Board
 * LCD SLC pin to arduino SLC pin
 * LCD SDA pin to arduino SDA pin

 created 23 July 2020
 by Forntoh Thomas

 This example is in the public domain.

 https://github.com/forntoh/LcdMenu/tree/master/examples/CharsetInput/CharsetInput.ino

*/

#include <ItemInput.h>
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

#define CHARSET_SIZE 10
// Create your charset
char charset[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
// Active index of the charset
uint8_t charsetPosition;

// Declare the call back function
void inputCallback(char* value);

MAIN_MENU(
    ITEM_INPUT("Con", inputCallback),
    ITEM_BASIC("Connect to WiFi"),
    ITEM_BASIC("Blink SOS"),
    ITEM_BASIC("Blink random")
);

LcdMenu menu(LCD_ROWS, LCD_COLS);

void setup() {
    Serial.begin(9600);
    menu.setupLcdWithMenu(0x27, mainMenu);
    // Initialize active index to zero
    charsetPosition = 0;
}

void loop() {
    if (!Serial.available()) return;
    char command = Serial.read();

    switch (command) {
        case UP:
            if (menu.isInEditMode())  // Update the position only in edit mode
                charsetPosition = (charsetPosition + 1) % CHARSET_SIZE;
            menu.drawChar(charset[charsetPosition]);  // Works only in edit mode
            menu.up();
            break;
        case DOWN:
            if (menu.isInEditMode())  // Update the position only in edit mode
                charsetPosition =
                    constrain(charsetPosition - 1, 0, CHARSET_SIZE);
            menu.drawChar(charset[charsetPosition]);  // Works only in edit mode
            menu.down();
            break;
        case LEFT:
            menu.left();
            break;
        case RIGHT:
            menu.right();
            break;
        case ENTER:  // Press enter to go to edit mode : for ItemInput
            menu.type(charset[charsetPosition]);  // Works only in edit mode
            menu.enter();
            break;
        case BACK:
            menu.back();
            break;
        case CLEAR:
            menu.clear();
            break;
        case BACKSPACE:  // Remove one character from tail
            menu.backspace();
            break;
        default:
            break;
    }
}
/**
 * Define callback
 */
void inputCallback(char* value) {
    // Do stuff with value
    Serial.print(F("# "));
    Serial.println(value);
}