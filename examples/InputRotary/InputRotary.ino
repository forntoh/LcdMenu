#include <ItemCommand.h>
#include <ItemInput.h>
#include <ItemInputCharsetSimple.h>
#include <ItemSubMenu.h>
#include <LcdMenu.h>
#include <SimpleRotary.h>
#include <interface/LiquidCrystalI2CAdapter.h>
#include <utils/RotaryNavConfig.h>

#define LCD_ROWS 2
#define LCD_COLS 16

#define CHARSET_SIZE 26
// Create your charset
char charset[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
// Active index of the charset
int8_t charsetPosition = -1;

// Declare the call back functions
void inputCallback(char *value);
void clearInput();

extern MenuItem *usernameMenu[];

MAIN_MENU(
    ITEM_SUBMENU("Set user", usernameMenu),
    ITEM_BASIC("Settings"),
    ITEM_BASIC("More Settings"),
    ITEM_BASIC("And more Settings"));

SUB_MENU(
    usernameMenu,
    mainMenu,
    ITEM_INPUT_CHARSET_SIMPLE("User", "", charset, CHARSET_SIZE, inputCallback),
    ITEM_COMMAND("Clear", clearInput));

LiquidCrystalI2CAdapter lcdAdapter(0x27, LCD_COLS, LCD_ROWS);
LcdMenu menu(lcdAdapter);

SimpleRotary encoder(2, 3, 4);

RotaryNavConfig menuConfig = {
    .encoder = &encoder,
    .menu = &menu,
    .longPressDuration = 1000,
};

void setup() {
    Serial.begin(9600);
    menu.initialize(mainMenu);
}

void loop() {
    // Call the handleRotaryMenu function, passing the menuConfig instance
    processWithRotaryEncoder(&menuConfig);
}

// Define the callbacks
void inputCallback(char *value) {
    // Do stuff with value
    Serial.print(F("# "));
    Serial.println(value);
}

void clearInput() {
    (static_cast<ItemInput*>(menu[1]))->setValue((char *)"");
}