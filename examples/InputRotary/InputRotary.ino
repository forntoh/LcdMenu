#include <ItemCommand.h>
#include <ItemInput.h>
#include <ItemInputCharset.h>
#include <ItemSubMenu.h>
#include <LcdMenu.h>
#include <MenuScreen.h>
#include <SimpleRotary.h>
#include <display/LiquidCrystal_I2CAdapter.h>
#include <input/SimpleRotaryAdapter.h>
#include <renderer/CharacterDisplayRenderer.h>

#define LCD_ROWS 2
#define LCD_COLS 16

// Create your charset
const char* charset = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

// Declare the call back functions
void inputCallback(char* value);
void clearInput();

extern MenuScreen* userScreen;

// clang-format off
MENU_SCREEN(mainScreen, mainItems,
    ITEM_SUBMENU("Set user", userScreen),
    ITEM_BASIC("Settings"),
    ITEM_BASIC("More Settings"),
    ITEM_BASIC("And more Settings"));

MENU_SCREEN(userScreen, userItems,
    ITEM_INPUT_CHARSET("User", charset, inputCallback),
    ITEM_COMMAND("Clear", clearInput));
// clang-format on

LiquidCrystal_I2C lcd(0x27, LCD_COLS, LCD_ROWS);
CharacterDisplayRenderer renderer(new LiquidCrystal_I2CAdapter(&lcd), LCD_COLS, LCD_ROWS);
LcdMenu menu(renderer);
SimpleRotary encoder(2, 3, 4);
SimpleRotaryAdapter rotaryInput(&menu, &encoder);

void setup() {
    Serial.begin(9600);
    renderer.begin();
    menu.setScreen(mainScreen);
}

void loop() { rotaryInput.observe(); }

// Define the callbacks
void inputCallback(char* value) {
    // Do stuff with value
    Serial.print(F("# "));
    Serial.println(value);
}

void clearInput() {
    if ((static_cast<ItemInput*>(userItems[0]))->setValue((char*)"")) {
        menu.refresh();
    }
}
