#include <ItemList.h>
#include <ItemRange.h>
#include <ItemToggle.h>
#include <LcdMenu.h>
#include <MenuScreen.h>
#include <SimpleRotary.h>
#include <display/LiquidCrystal_I2CAdapter.h>
#include <input/SimpleRotaryAdapter.h>
#include <renderer/CharacterDisplayRenderer.h>

#define LCD_ROWS 2
#define LCD_COLS 16

// Declare the callbacks
void callback(int pos);
void toggleBacklight(bool isOn);

std::vector<const char*> colors = {"Red", "Green", "Blue", "Orange", "Aqua", "Yellow", "Purple", "Pink"};

// clang-format off
MENU_SCREEN(mainScreen, mainItems,
    ITEM_BASIC("Connect to WiFi"),
    ITEM_LIST("Color", colors, [](const uint8_t color) { Serial.println(colors[color]); }),
    ITEM_BASIC("Blink SOS"),
    ITEM_RANGE<int>("Dist", 0, 1, 0, 50, callback, "%dm", 1, true),
    ITEM_TOGGLE("Backlight", toggleBacklight),
    ITEM_BASIC("Blink random"));
// clang-format on

LiquidCrystal_I2C lcd(0x27, LCD_COLS, LCD_ROWS);
LiquidCrystal_I2CAdapter lcdAdapter(&lcd);
CharacterDisplayRenderer renderer(&lcdAdapter, LCD_COLS, LCD_ROWS);
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
void toggleBacklight(bool isOn) {
    lcdAdapter.setBacklight(isOn);
}

void callback(int pos) {
    Serial.println(pos);
}
