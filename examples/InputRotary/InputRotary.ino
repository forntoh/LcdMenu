#include <ItemCommand.h>
#include <ItemInput.h>
#include <ItemInputCharset.h>
#include <ItemSubMenu.h>
#include <LcdMenu.h>
#include <SimpleRotary.h>
#include <input/SimpleRotaryAdapter.h>
#include <interface/LiquidCrystalI2CAdapter.h>

#define LCD_ROWS 2
#define LCD_COLS 16

// Create your charset
const char* charset = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

// Declare the call back functions
void inputCallback(char* value);
void clearInput();

extern MenuItem* usernameMenu[];

MAIN_MENU(
    ITEM_SUBMENU("Set user", usernameMenu),
    ITEM_BASIC("Settings"),
    ITEM_BASIC("More Settings"),
    ITEM_BASIC("And more Settings"));

SUB_MENU(
    usernameMenu,
    mainMenu,
    ITEM_INPUT_CHARSET("User", charset, inputCallback),
    ITEM_COMMAND("Clear", clearInput));

SimpleRotary encoder(2, 3, 4);

LiquidCrystalI2CAdapter lcdAdapter(0x27, LCD_COLS, LCD_ROWS);
LcdMenu menu(lcdAdapter);
SimpleRotaryAdapter rotaryInput(&menu, &encoder);

void setup() {
    Serial.begin(9600);
    menu.initialize(mainMenu);
}

void loop() { rotaryInput.observe(); }

// Define the callbacks
void inputCallback(char* value) {
    // Do stuff with value
    Serial.print(F("# "));
    Serial.println(value);
}

void clearInput() {
    (static_cast<ItemInput*>(menu[1]))->setValue((char*)"");
}