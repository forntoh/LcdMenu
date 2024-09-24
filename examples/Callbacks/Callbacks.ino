/*
 Menu Item Callbacks

 https://lcdmenu.forntoh.dev/examples/command-and-toggle

*/

#include <ItemToggle.h>
#include <LcdMenu.h>
#include <MenuScreen.h>
#include <display/LiquidCrystal_I2CAdapter.h>
#include <input/KeyboardAdapter.h>

#define LCD_ROWS 2
#define LCD_COLS 16

// Declare the call back function
void toggleBacklight(uint8_t isOn);

// clang-format off
MENU_SCREEN(mainScreen, mainItems,
    ITEM_BASIC("Start service"),
    ITEM_BASIC("Connect to WiFi"),
    ITEM_TOGGLE("Backlight", toggleBacklight),
    ITEM_BASIC("Blink SOS"),
    ITEM_BASIC("Blink random"));
// clang-format on

LiquidCrystal_I2C lcd(0x27, LCD_COLS, LCD_ROWS);
LiquidCrystal_I2CAdapter lcdAdapter(&lcd, LCD_COLS, LCD_ROWS);
LcdMenu menu(lcdAdapter);
KeyboardAdapter keyboard(&menu, &Serial);

void setup() {
    Serial.begin(9600);
    lcdAdapter.begin();
    menu.setScreen(mainScreen);
}

void loop() {
    keyboard.observe();
}

/**
 * Define callback
 */
void toggleBacklight(uint16_t isOn) {
    lcdAdapter.setBacklight(isOn);
}