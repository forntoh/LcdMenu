/*
 Simple Input

 https://lcdmenu.forntoh.dev/examples/simple-input

*/

#include <ItemInput.h>
#include <LcdMenu.h>
#include <interface/LiquidCrystalI2CAdapter.h>
#include <utils/commandProccesors.h>

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

// Declare the call back function
void inputCallback(char* value);

MAIN_MENU(
    ITEM_INPUT("Con", inputCallback), 
    ITEM_BASIC("Connect to WiFi"),
    ITEM_BASIC("Blink SOS"), 
    ITEM_BASIC("Blink random")
);

LiquidCrystalI2CAdapter lcdAdapter(0x27, LCD_COLS, LCD_ROWS);
LcdMenu menu(lcdAdapter);

void setup() {
    Serial.begin(9600);
    menu.initialize(mainMenu);
}

void loop() {
    if (!Serial.available()) return;
    char command = Serial.read();
    processMenuCommand(menu, command, UP, DOWN, LEFT, RIGHT, ENTER, BACK, CLEAR,
                       BACKSPACE);
}
/**
 * Define callback
 */
void inputCallback(char* value) {
    Serial.print(F("# "));
    Serial.println(value);
}