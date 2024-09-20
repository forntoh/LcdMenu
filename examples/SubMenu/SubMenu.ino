/*
 Sub Menu

 https://lcdmenu.forntoh.dev/examples/submenu

*/
#include <ItemSubMenu.h>
#include <LcdMenu.h>
#include <MenuScreen.h>
#include <display/LiquidCrystal_I2CAdapter.h>
#include <utils/SimpleNavConfig.h>

#define LCD_ROWS 2
#define LCD_COLS 16

// clang-format off
MENU_SCREEN(settings2Screen, settings2Items,
    ITEM_BASIC("Backlight"),
    ITEM_BASIC("Contrast"));
// clang-format on

// Create submenu and precise its parent
// clang-format off
MENU_SCREEN(settingsScreen, settingsItems,
    ITEM_BASIC("Backlight"),
    ITEM_BASIC("Contrast"),
    ITEM_BASIC("Contrast1"),
    ITEM_BASIC("Contrast2"),
    ITEM_BASIC("Contrast3"),
    ITEM_BASIC("Contrast4"),
    ITEM_BASIC("Contrast5"),
    ITEM_BASIC("Contrast6"),
    ITEM_BASIC("Contrast7"),
    ITEM_SUBMENU("Settings2", settings2Screen));
// clang-format on

// Define the main menu
// clang-format off
MENU_SCREEN(mainScreen, mainItems,
    ITEM_SUBMENU("Settings", settingsScreen),
    ITEM_BASIC("Start service"),
    ITEM_BASIC("Connect to WiFi"),
    ITEM_BASIC("Blink SOS"),
    ITEM_BASIC("Blink random"),
    ITEM_SUBMENU("Settings 2", settings2Screen));
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
    .left = NULL,
    .right = NULL,
};

void setup() {
    Serial.begin(9600);
    menu.initialize(mainScreen);
}

void loop() {
    if (!Serial.available()) return;
    char command = Serial.read();
    processWithSimpleCommand(&navConfig, command);
}
