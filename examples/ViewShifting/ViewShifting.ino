/**
 * This example demostrates how the view shifting feature works.
 * The view shifting feature allows the user to scroll through an item that is too long to fit on the screen.
 * The user can scroll the view to the left or right to see the entire item.
 */
#include <LcdMenu.h>
#include <MenuScreen.h>
#include <display/LiquidCrystal_I2CAdapter.h>
#include <input/KeyboardAdapter.h>
#include <renderer/CharacterDisplayRenderer.h>

#define LCD_ROWS 2
#define LCD_COLS 16

// clang-format off
MENU_SCREEN(mainScreen, mainItems,
    ITEM_BASIC("I'm short"),
    ITEM_BASIC("I'm a very long item that doesn't fit on the screen"),
    ITEM_BASIC("I'm another very long item that doesn't fit on the screen"),
    ITEM_BASIC("I'm quite long too"));
// clang-format on

LiquidCrystal_I2C lcd(0x27, LCD_COLS, LCD_ROWS);
CharacterDisplayRenderer renderer(new LiquidCrystal_I2CAdapter(&lcd), LCD_COLS, LCD_ROWS);
LcdMenu menu(renderer);
KeyboardAdapter keyboard(&menu, &Serial);

void setup() {
    Serial.begin(9600);
    renderer.begin();
    menu.setScreen(mainScreen);
}

void loop() {
    keyboard.observe();
}