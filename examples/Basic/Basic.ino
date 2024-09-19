/*
 Basic Menu

 https://lcdmenu.forntoh.dev/examples/basic

*/

#include <LcdMenu.h>
#include <MenuScreen.h>
#include <display/LiquidCrystal_I2CAdapter.h>
#include <utils/SimpleNavConfig.h>

#define LCD_ROWS 2
#define LCD_COLS 16

// Initialize the main menu items
MENU_SCREEN(mainScreen, mainItems,
    ITEM_BASIC("Start service"),
    ITEM_BASIC("Connect to WiFi"),
    ITEM_BASIC("Settings"),
    ITEM_BASIC("Blink SOS"),
    ITEM_BASIC("Blink random"));

// Construct the display adapter
LiquidCrystal_I2C lcd(0x27, LCD_COLS, LCD_ROWS);
LiquidCrystal_I2CAdapter lcdAdapter(&lcd, LCD_COLS, LCD_ROWS);

// Construct the LcdMenu
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
    menu.initialize(mainScreen);
}

void loop() {
    if (!Serial.available()) return;
    char command = Serial.read();
    processWithSimpleCommand(&navConfig, command);
}