#include <ItemWidget.h>
#include <LcdMenu.h>
#include <MenuScreen.h>
#include <display/LiquidCrystal_I2CAdapter.h>
#include <input/KeyboardAdapter.h>
#include <renderer/CharacterDisplayRenderer.h>
#include <widget/WidgetRange.h>

#define LCD_ROWS 2
#define LCD_COLS 16

// Declare the callbacks
void callbackInt(int value);
void callbackFloat(float value);

// Initialize the main menu items
// clang-format off
MENU_SCREEN(mainScreen, mainItems,
    ITEM_BASIC("Con"),
    ITEM_WIDGET(
        "Dist",
        callbackInt,
        WIDGET_RANGE(100, 1, 100, 200, "%dm", 1)),
    ITEM_WIDGET(
        "Curr",
        callbackFloat,
        WIDGET_RANGE(-1.0f, 0.01f, -1.0f, 1.0f, "%.2fmA", 2)),
    ITEM_BASIC("Blink SOS"),
    ITEM_BASIC("Blink random"));
// clang-format on

// Construct the LcdMenu
LiquidCrystal_I2C lcd(0x27, LCD_COLS, LCD_ROWS);
CharacterDisplayRenderer renderer(new LiquidCrystal_I2CAdapter(&lcd), LCD_COLS, LCD_ROWS);
LcdMenu menu(renderer);
KeyboardAdapter keyboard(&menu, &Serial);

void setup() {
    Serial.begin(9600);
    // Initialize LcdMenu with the menu items
    renderer.begin();
    menu.setScreen(mainScreen);
}

void loop() {
    keyboard.observe();
}

void callbackInt(int value) {
    // do something with the integer value
    Serial.println(value);
}

void callbackFloat(float value) {
    // do something with the float value
    Serial.println(value);
}
