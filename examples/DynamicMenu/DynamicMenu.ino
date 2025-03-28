#include "utils/custom_printf.h"
#include <ItemCommand.h>
#include <ItemSubMenu.h>
#include <LcdMenu.h>
#include <MenuScreen.h>
#include <display/LiquidCrystal_I2CAdapter.h>
#include <input/KeyboardAdapter.h>
#include <renderer/CharacterDisplayRenderer.h>

#define LCD_ROWS 2
#define LCD_COLS 16

// Declare the main menu and sub-menu
extern MenuScreen* settingsScreen;

LiquidCrystal_I2C lcd(0x27, LCD_COLS, LCD_ROWS);
LiquidCrystal_I2CAdapter lcdAdapter(&lcd);
CharacterDisplayRenderer renderer(&lcdAdapter, LCD_COLS, LCD_ROWS);
LcdMenu menu(renderer);
KeyboardAdapter keyboard(&menu, &Serial);

// clang-format off
MENU_SCREEN(mainScreen, mainItems,
    ITEM_BASIC("Placeholder 1"),
    ITEM_SUBMENU("Dynamic >", settingsScreen),
    ITEM_BASIC("Placeholder 2"),
    ITEM_BASIC("Placeholder 3"));

MENU_SCREEN(settingsScreen, settingsItems,
    ITEM_COMMAND("Add network", []() {
        // Dynamically add items to the screen
        settingsScreen->addItem(ITEM_BASIC("Network Added"));
        menu.refresh();
    }),
    ITEM_COMMAND("Add between", []() {
        // Dynamically add items to the screen at a specific position
        settingsScreen->addItem(1, ITEM_BASIC("Network Between"));
        menu.refresh();
    }),
    ITEM_COMMAND("Remove last", []() {
        // Dynamically remove the last item from the screen
        settingsScreen->removeLastItem();
        menu.refresh();
    }),
    ITEM_COMMAND("Remove between", []() {
        // Dynamically remove an item from the screen at a specific position
        settingsScreen->removeItem(1);
        menu.refresh();
    }));
// clang-format on

void setup() {
    Serial.begin(9600);
    renderer.begin();
    menu.setScreen(mainScreen);
}

void loop() {
    keyboard.observe();
}