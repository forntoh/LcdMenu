/*
 Menu Item Callbacks

 https://lcdmenu.forntoh.dev/examples/command-and-toggle

*/

#include <ItemToggle.h>
#include <LcdMenu.h>
#include <interface/LiquidCrystal_I2CAdapter.h>
#include <utils/SimpleNavConfig.h>

#define LCD_ROWS 2
#define LCD_COLS 16

// Declare the call back function
void toggleBacklight(uint16_t isOn);

// prettier-ignore
MAIN_MENU(
    ITEM_BASIC("Start service"),
    ITEM_BASIC("Connect to WiFi"),
    ITEM_TOGGLE("Backlight", toggleBacklight),
    ITEM_BASIC("Blink SOS"),
    ITEM_BASIC("Blink random"));

LiquidCrystal_I2C lcd(0x27, LCD_COLS, LCD_ROWS);
LiquidCrystal_I2CAdapter lcdAdapter(&lcd, LCD_COLS, LCD_ROWS);
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
/**
 * Define callback
 */
void toggleBacklight(uint16_t isOn) { lcdAdapter.setBacklight(isOn); }
