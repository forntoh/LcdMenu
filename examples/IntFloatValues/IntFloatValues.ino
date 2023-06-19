/*
 Menu Item Progress

 https://lcdmenu.forntoh.dev/examples/string-list

*/

#include <ItemProgress.h>
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

// Declare the calbacks
void callback(uint8_t pos);

char* intMapping(uint16_t progress) {
    // Map the progress value to a new range (100 to 200)
    long mapped = mapProgress(progress, 100, 200);

    // Buffer to store the converted stringV
    static char buffer[10];

    // Convert the mapped value to a string
    itoa(mapped, buffer, 10);

    // Concatenate the string with the character 'm'
    concat(buffer, 'm', buffer);

    // Return the resulting string
    return buffer;
}

char* floatMapping(uint16_t progress) {
    // Normalize the progress value and map it to the specified floating-point
    // range
    float floatValue = mapProgress(progress, -1.0f, 1.0f);

    // Buffer to store the converted string
    static char buffer[10];

    // Convert the floating-point value to a string with 4 characters (including
    // decimal point) and 2 decimal places
    dtostrf(floatValue, 4, 2, buffer);

    // Concatenate the string with the character 'c'
    concat(buffer, 'c', buffer);

    // Return the resulting string
    return buffer;
}

// Initialize the main menu items
MAIN_MENU(ITEM_BASIC("Con"), ITEM_PROGRESS("Dist", 1, intMapping, callback),
          ITEM_BASIC("Blink SOS"), ITEM_BASIC("Blink random"));

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
}

void callback(uint8_t pos) {
    // do something with the progress
    Serial.println(pos);
}
