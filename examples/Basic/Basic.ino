/*
 Basic Menu

 https://lcdmenu.forntoh.dev/examples/basic

*/

#include <LcdMenu.h>
#include <interface/LiquidCrystalI2CAdapter.h>
#include <utils/commandProccesors.h>

#define LCD_ROWS 2
#define LCD_COLS 16

// Configure keyboard keys (ASCII)
#define UP 56     // NUMPAD 8
#define DOWN 50   // NUMPAD 2
#define ENTER 53  // NUMPAD 5
#define BACK 55   // NUMPAD 7

// Initialize the main menu items
MAIN_MENU(ITEM_BASIC("Start service"), ITEM_BASIC("Connect to WiFi"),
          ITEM_BASIC("Settings"), ITEM_BASIC("Blink SOS"),
          ITEM_BASIC("Blink random"));
// Construct the LcdMenu
LiquidCrystalI2CAdapter lcdAdapter(0x27, LCD_COLS, LCD_ROWS);
LcdMenu menu(lcdAdapter);

void setup() {
    Serial.begin(9600);
    // Initialize LcdMenu with the menu items
    menu.initialize(mainMenu);
}

void loop() {
    if (!Serial.available()) return;
    char command = Serial.read();
    processMenuCommand(menu, command, UP, DOWN, ENTER, BACK);
}