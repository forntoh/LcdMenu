#include <ItemList.h>
#include <ItemToggle.h>
#include <ItemWidget.h>
#include <LcdMenu.h>
#include <MenuScreen.h>
#include <SimpleRotary.h>
#include <display/LiquidCrystal_I2CAdapter.h>
#include <input/SimpleRotaryAdapter.h>
#include <renderer/CharacterDisplayRenderer.h>
#include <widget/WidgetList.h>
#include <widget/WidgetRange.h>

#define LCD_ROWS 2
#define LCD_COLS 16

// Declare the callbacks
void callback(int pos);
void toggleBacklight(bool isOn);

static const char* colors[] = {"Red", "Green", "Blue", "Orange", "Aqua", "Yellow", "Purple", "Pink"};
static const uint8_t COLORS_COUNT = sizeof(colors) / sizeof(colors[0]);

// clang-format off
MENU_SCREEN(mainScreen, mainItems,
    ITEM_BASIC("Connect to WiFi"),
    ITEM_WIDGET(
        "Color",
        [](const size_t color) { Serial.println(colors[color]); },
        WIDGET_LIST(colors, COLORS_COUNT, 0, "%s")),
    ITEM_BASIC("Blink SOS"),
    ITEM_WIDGET(
        "Dist",
        callback,
        WIDGET_RANGE(0, 1, 0, 50, "%dm", 1, true)),
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
