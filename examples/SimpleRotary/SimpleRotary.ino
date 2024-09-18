#include <ItemList.h>
#include <ItemProgress.h>
#include <ItemToggle.h>
#include <LcdMenu.h>
#include <SimpleRotary.h>
#include <display/LiquidCrystal_I2CAdapter.h>
#include <input/SimpleRotaryAdapter.h>

#define LCD_ROWS 2
#define LCD_COLS 16

// Declare the callbacks
void callback(uint16_t pos);
void colorsCallback(uint16_t pos);
void toggleBacklight(uint16_t isOn);

char* intMapping(uint16_t progress) {
    long mapped = mapProgress(progress, 100L, 200L);
    static char buffer[10];
    itoa(mapped, buffer, 10);
    concat(buffer, 'm', buffer);
    return buffer;
}

String colors[] = {"Red", "Green", "Blue", "Orange", "Aqua", "Yellow", "Purple", "Pink"};

MAIN_MENU(
    ITEM_BASIC("Connect to WiFi"),
    ITEM_STRING_LIST("Color", colors, 8, colorsCallback),
    ITEM_BASIC("Blink SOS"),
    ITEM_PROGRESS("Dist", 10, intMapping, callback),
    ITEM_TOGGLE("Backlight", toggleBacklight),
    ITEM_BASIC("Blink random"));

LiquidCrystal_I2C lcd(0x27, LCD_COLS, LCD_ROWS);
LiquidCrystal_I2CAdapter lcdAdapter(&lcd, LCD_COLS, LCD_ROWS);
LcdMenu menu(lcdAdapter);
SimpleRotary encoder(2, 3, 4);
SimpleRotaryAdapter rotaryInput(&menu, &encoder);

void setup() {
    Serial.begin(9600);
    menu.initialize(mainMenu);
}

void loop() { rotaryInput.observe(); }

// Define the callbacks
void toggleBacklight(uint16_t isOn) {
    lcdAdapter.setBacklight(isOn);
}

void callback(uint16_t pos) {
    Serial.println(pos);
}

void colorsCallback(uint16_t pos) {
    Serial.println(colors[pos]);
}