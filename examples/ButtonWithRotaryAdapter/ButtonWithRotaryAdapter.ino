// Disable double press detection
#define DOUBLE_PRESS_THRESHOLD 0

#include <Button.h>
#include <ItemInputCharset.h>
#include <ItemList.h>
#include <ItemToggle.h>
#include <LcdMenu.h>
#include <MenuScreen.h>
#include <SimpleRotary.h>
#include <display/LiquidCrystal_I2CAdapter.h>
#include <input/ButtonAdapter.h>
#include <input/SimpleRotaryAdapter.h>
#include <renderer/CharacterDisplayRenderer.h>

#define LCD_ROWS 2
#define LCD_COLS 16

// Declare the callbacks
void colorsCallback(uint8_t pos);
void toggleBacklight(bool isOn);
void inputCallback(char* value);

String colors[] = {"Red", "Green", "Blue", "Orange", "Aqua", "Yellow", "Purple", "Pink"};

// clang-format off
MENU_SCREEN(mainScreen, mainItems,
    ITEM_INPUT_CHARSET("User", (const char*)"ABCDEFGHIJKLMNOPQRSTUVWXYZ", inputCallback),
    ITEM_STRING_LIST("Color", colors, 8, colorsCallback),
    ITEM_TOGGLE("Backlight", toggleBacklight),
    ITEM_BASIC("Placeholder 1"),
    ITEM_BASIC("Placeholder 2"));
// clang-format on

LiquidCrystal_I2C lcd(0x27, LCD_COLS, LCD_ROWS);
LiquidCrystal_I2CAdapter lcdAdapter(&lcd);
CharacterDisplayRenderer renderer(&lcdAdapter, LCD_COLS, LCD_ROWS);
LcdMenu menu(renderer);
SimpleRotary encoder(2, 3, 4);
SimpleRotaryAdapter rotaryInput(&menu, &encoder);  // Rotary input adapter
Button backspaceBtn(11);
ButtonAdapter backspaceBtnA(&menu, &backspaceBtn, BACKSPACE);  // Push button for backspace

void setup() {
    backspaceBtn.begin();
    renderer.begin();
    Serial.begin(9600);
    menu.setScreen(mainScreen);
}

void loop() {
    rotaryInput.observe();
    backspaceBtnA.observe();
}

// Define the callbacks
void toggleBacklight(bool isOn) {
    lcdAdapter.setBacklight(isOn);
}

void colorsCallback(uint8_t pos) {
    Serial.println(colors[pos]);
}

void inputCallback(char* value) {
    Serial.println(value);
}