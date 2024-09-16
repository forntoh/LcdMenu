/*
 Simple Input

 https://lcdmenu.forntoh.dev/examples/simple-input

*/

#include <ItemInput.h>
#include <LcdMenu.h>
#include <interface/LiquidCrystalI2CAdapter.h>
#include <utils/SimpleNavConfig.h>

#define LCD_ROWS 2
#define LCD_COLS 16

// Declare the call back function
void inputCallback(char* value);

MAIN_MENU(
    ITEM_INPUT("Con", inputCallback),
    ITEM_BASIC("Connect to WiFi"),
    ITEM_BASIC("Blink SOS"),
    ITEM_BASIC("Blink random"));

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
    .clear = 'c',
    .backspace = 'v',
};

void setup() {
    Serial.begin(9600);
    menu.initialize(mainMenu);
}

void loop() {
    if (!Serial.available()) return;
    char command = Serial.read();

    if (!processWithSimpleCommand(&navConfig, command) && command != '\n') {
        menu.process(command);
    }
}
/**
 * Define callback
 */
void inputCallback(char* value) {
    Serial.print(F("# "));
    Serial.println(value);
}
