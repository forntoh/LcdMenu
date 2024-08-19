/*
 Menu Item Callbacks

 https://lcdmenu.forntoh.dev/examples/command-and-toggle

*/

#include <ItemToggle.h>
#include <LcdMenu.h>
#include <interface/LiquidCrystalI2CAdapter.h>
#include <utils/commandProccesors.h>

#define LCD_ROWS 2
#define LCD_COLS 16

// Configure keyboard keys (ASCII)
#define UP 'w'
#define DOWN 's'
#define LEFT 'a'
#define RIGHT 'd'
#define ENTER ' '
#define BACK 'b'
#define BACKSPACE 'v'
#define CLEAR 'c'

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

LiquidCrystalI2CAdapter lcdAdapter(0x27, LCD_COLS, LCD_ROWS);
LcdMenu menu(lcdAdapter);

void setup() {
    Serial.begin(9600);
    menu.initialize(mainMenu);
}

void loop() {
    if (!Serial.available()) return;
    char command = Serial.read();
    processMenuCommand(menu, command, UP, DOWN, ENTER, BACK);
}
/**
 * Define callback
 */
void toggleBacklight(uint16_t isOn) { lcdAdapter.lcd.setBacklight(isOn); }
