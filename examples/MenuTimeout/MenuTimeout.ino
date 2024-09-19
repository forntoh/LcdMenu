/*
 Menu Timeout

 https://lcdmenu.forntoh.dev/examples/timeout

*/

#include <LcdMenu.h>
#include <MenuScreen.h>
#include <display/LiquidCrystal_I2CAdapter.h>
#include <utils/SimpleNavConfig.h>

#define LCD_ROWS 2
#define LCD_COLS 16

// clang-format off
MENU_SCREEN(mainScreen, mainItems,
    ITEM_BASIC("Start service"),
    ITEM_BASIC("Connect to WiFi"),
    ITEM_BASIC("Settings"),
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
};

void setup() {
    Serial.begin(9600);
    menu.initialize(mainScreen);
}

void loop() {
    /**
     * IMPORTANT: You must call this function for the timeout to work
     * The default timeout is 10000ms
     */
    lcdAdapter.updateTimer();

    // Listen to key
    if (!Serial.available()) return;
    char command = Serial.read();

    processWithSimpleCommand(&navConfig, command);
}