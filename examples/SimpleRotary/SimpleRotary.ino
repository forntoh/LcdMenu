#include <ItemList.h>
#include <ItemProgress.h>
#include <ItemToggle.h>
#include <ItemInputCharsetSimple.h>
#include <LcdMenu.h>
#include <SimpleRotary.h>
#include <interface/LiquidCrystalI2CAdapter.h>
#include <utils/RotaryNavConfig.h>

#define LCD_ROWS 2
#define LCD_COLS 16

// Declare the callbacks
void callback(uint16_t pos);
void colorsCallback(uint16_t pos);
void toggleBacklight(uint16_t isOn);

char *intMapping(uint16_t progress) {
    long mapped = mapProgress(progress, 100L, 200L);
    static char buffer[10];
    itoa(mapped, buffer, 10);
    concat(buffer, 'm', buffer);
    return buffer;
}

String colors[] = {"Red", "Green", "Blue", "Orange", "Aqua", "Yellow", "Purple", "Pink"};

MAIN_MENU(
    ITEM_BASIC("Connect to WiFi"),
    ITEM_INPUT_CHARSET_SIMPLE("Con", "6525637", "0123456789", 10, [](char *value) { Serial.println(value); }),
    ITEM_STRING_LIST("Color", colors, 8, colorsCallback),
    ITEM_BASIC("Blink SOS"),
    ITEM_PROGRESS("Dist", 10, intMapping, callback),
    ITEM_TOGGLE("Backlight", toggleBacklight),
    ITEM_BASIC("Blink random"));

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
void toggleBacklight(uint16_t isOn) {
    lcdAdapter.setBacklight(isOn);
}

void callback(uint16_t pos) {
    Serial.println(pos);
}

void colorsCallback(uint16_t pos) {
    Serial.println(colors[pos]);
}