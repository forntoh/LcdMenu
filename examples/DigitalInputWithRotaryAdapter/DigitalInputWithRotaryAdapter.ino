// Disable double press detection
#define DOUBLE_PRESS_THRESHOLD 0

#include <ItemInputCharset.h>
#include <ItemList.h>
#include <ItemToggle.h>
#include <LcdMenu.h>
#include <SimpleRotary.h>
#include <display/LiquidCrystal_I2CAdapter.h>
#include <input/DigitalInputAdapter.h>
#include <input/SimpleRotaryAdapter.h>

#define LCD_ROWS 2
#define LCD_COLS 16

// Declare the callbacks
void colorsCallback(uint16_t pos);
void toggleBacklight(uint16_t isOn);
void inputCallback(char* value);

String colors[] = {"Red", "Green", "Blue", "Orange", "Aqua", "Yellow", "Purple", "Pink"};

MAIN_MENU(
    ITEM_INPUT_CHARSET("User", (const char*)"ABCDEFGHIJKLMNOPQRSTUVWXYZ", inputCallback),
    ITEM_STRING_LIST("Color", colors, 8, colorsCallback),
    ITEM_TOGGLE("Backlight", toggleBacklight),
    ITEM_BASIC("Placeholder 1"),
    ITEM_BASIC("Placeholder 2"));

LiquidCrystal_I2C lcd(0x27, LCD_COLS, LCD_ROWS);
LiquidCrystal_I2CAdapter lcdAdapter(&lcd, LCD_COLS, LCD_ROWS);
LcdMenu menu(lcdAdapter);
SimpleRotary encoder(2, 3, 4);
SimpleRotaryAdapter rotaryInput(&menu, &encoder);       // Rotary input adapter
DigitalInputAdapter backspaceBtn(&menu, 5, BACKSPACE);  // Push button for backspace

void setup() {
    Serial.begin(9600);
    menu.initialize(mainMenu);
}

void loop() {
    rotaryInput.observe();
    backspaceBtn.observe();
}

// Define the callbacks
void toggleBacklight(uint16_t isOn) {
    lcdAdapter.setBacklight(isOn);
}

void colorsCallback(uint16_t pos) {
    Serial.println(colors[pos]);
}

void inputCallback(char* value) {
    Serial.println(value);
}