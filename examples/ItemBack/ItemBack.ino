#include <ItemBack.h>
#include <ItemSubMenu.h>
#include <LcdMenu.h>
#include <MenuScreen.h>
#include <display/LiquidCrystal_I2CAdapter.h>
#include <utils/SimpleNavConfig.h>

#define LCD_ROWS 2
#define LCD_COLS 16

extern MenuItem* settingsMenu[];

// Create submenu and precise its parent
// clang-format off
MENU_SCREEN(settingsScreen, settingsItems,
    ITEM_BASIC("Backlight"),
    ITEM_BASIC("Contrast"),
    ITEM_BACK());
// clang-format on

// Define the main menu
// clang-format off
MENU_SCREEN(mainScreen, mainItems,
    ITEM_BASIC("Start service"),
    ITEM_BASIC("Connect to WiFi"),
    ITEM_SUBMENU("Settings", settingsScreen),
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
    .left = NULL,
    .right = NULL,
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