/*
 Sub Menu

 https://lcdmenu.forntoh.dev/examples/submenu

*/
#include <ItemSubMenu.h>
#include <LcdMenu.h>
#include <interface/LiquidCrystalI2CAdapter.h>
#include <utils/SimpleNavConfig.h>
#include <utils/commandProccesors.h>

#define LCD_ROWS 2
#define LCD_COLS 16

extern MenuItem *settingsMenu[];

// Define the main menu
MAIN_MENU(
    ITEM_BASIC("Start service"),
    ITEM_BASIC("Connect to WiFi"),
    ITEM_SUBMENU("Settings", settingsMenu),
    ITEM_BASIC("Blink SOS"),
    ITEM_BASIC("Blink random"));
/**
 * Create submenu and precise its parent
 */
SUB_MENU(
    settingsMenu,
    mainMenu,
    ITEM_BASIC("Backlight"),
    ITEM_BASIC("Contrast"));

LiquidCrystalI2CAdapter lcdAdapter(0x27, LCD_COLS, LCD_ROWS);
LcdMenu menu(lcdAdapter);

SimpleNavConfig navConfig = {
    .menu = &menu,
    .up = 'w',
    .down = 's',
    .enter = ' ',
    .back = 'b',
    .left = NULL,
    .right = NULL,
};

void setup() {
    Serial.begin(9600);
    menu.initialize(mainMenu);
}

void loop() {
    if (!Serial.available()) return;
    char command = Serial.read();
    processWithSimpleCommand(&navConfig, command);
}