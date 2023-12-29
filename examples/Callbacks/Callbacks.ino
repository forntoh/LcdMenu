/*
 Menu Item Callbacks

 https://lcdmenu.forntoh.dev/examples/command-and-toggle

*/

#include <ItemToggle.h>
#include <controller/LiquidCrystalI2CMenu.h>
#include <utils/commandProccesors.h>

#define LCD_ROWS 2
#define LCD_COLS 16

// Configure keyboard keys (ASCII)
#define UP 56     // NUMPAD 8
#define DOWN 50   // NUMPAD 2
#define ENTER 53  // NUMPAD 5
#define BACK 55   // NUMPAD 7

// Declare the call back function
void toggleBacklight(uint16_t isOn);

// prettier-ignore
MAIN_MENU(
    ITEM_BASIC("Start service"),
    ITEM_BASIC("Connect to WiFi"),
    ITEM_TOGGLE("Backlight", toggleBacklight),
    ITEM_BASIC("Blink SOS"),
    ITEM_BASIC("Blink random")
);

LiquidCrystalI2CMenu menu(LCD_ROWS, LCD_COLS);

void setup() {
    Serial.begin(9600);
    menu.setupLcdWithMenu(0x27, mainMenu);
}

void loop() {
    if (!Serial.available()) return;
    char command = Serial.read();
    processMenuCommand(menu, command, UP, DOWN, ENTER, BACK);
}
/**
 * Define callback
 */
void toggleBacklight(uint16_t isOn) { menu.setBacklight(isOn); }
