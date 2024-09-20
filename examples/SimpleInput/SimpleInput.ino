/*
 Simple Input

 https://lcdmenu.forntoh.dev/examples/simple-input

*/

#include <ItemInput.h>
#include <LcdMenu.h>
#include <MenuScreen.h>
#include <display/LiquidCrystal_I2CAdapter.h>
#include <utils/SimpleNavConfig.h>

#define LCD_ROWS 2
#define LCD_COLS 16

// Declare the call back function
void inputCallback(char* value);

// clang-format off
MENU_SCREEN(mainScreen, mainItems,
    ITEM_INPUT("Con", inputCallback),
    ITEM_BASIC("Connect to WiFi"),
    ITEM_BASIC("Blink SOS"),
    ITEM_BASIC("Blink random"));
// clang-format on

LiquidCrystal_I2C lcd(0x27, LCD_COLS, LCD_ROWS);
LiquidCrystal_I2CAdapter lcdAdapter(&lcd, LCD_COLS, LCD_ROWS);
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
    lcdAdapter.begin();
    menu.setScreen(mainScreen);
}

void loop() {
    if (!Serial.available()) return;
    char command = Serial.read();
    processWithSimpleCommand(&navConfig, command);
}
/**
 * Define callback
 */
void inputCallback(char* value) {
    Serial.print(F("# "));
    Serial.println(value);
}
