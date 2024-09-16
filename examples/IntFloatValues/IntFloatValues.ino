/*
 Menu Item Progress

 https://lcdmenu.forntoh.dev/examples/string-list

*/

#include <ItemProgress.h>
#include <LcdMenu.h>
#include <interface/LiquidCrystalI2CAdapter.h>
#include <utils/SimpleNavConfig.h>

#define LCD_ROWS 2
#define LCD_COLS 16

// Declare the callbacks
void callback(uint16_t pos);

char* intMapping(uint16_t progress) {
    // Map the progress value to a new range (100 to 200)
    long mapped = mapProgress(progress, 100L, 200L);

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

    // Concatenate the string with the character 'A'
    concat(buffer, 'A', buffer);

    // Return the resulting string
    return buffer;
}

// Initialize the main menu items
MAIN_MENU(
    ITEM_BASIC("Con"),
    ITEM_PROGRESS("Dist", 10, intMapping, callback),
    ITEM_PROGRESS("Curr", 5, floatMapping, callback),
    ITEM_BASIC("Blink SOS"),
    ITEM_BASIC("Blink random"));

// Construct the LcdMenu
LiquidCrystalI2CAdapter lcdAdapter(0x27, LCD_COLS, LCD_ROWS);
LcdMenu menu(lcdAdapter);

SimpleNavConfig navConfig = {
    .menu = &menu,
    .up = 'w',
    .down = 's',
    .enter = ' ',
    .back = 'b',
    .left = 'a',
    .right = 'd',
};

void setup() {
    Serial.begin(9600);
    // Initialize LcdMenu with the menu items
    menu.initialize(mainMenu);
}

void loop() {
    if (!Serial.available()) return;
    char command = Serial.read();
    processWithSimpleCommand(&navConfig, command);
}

void callback(uint16_t pos) {
    // do something with the progress
    Serial.println(pos);
}
