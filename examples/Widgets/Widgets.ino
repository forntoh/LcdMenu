#include <ItemWidget.h>
#include <LcdMenu.h>
#include <MenuScreen.h>
#include <display/LiquidCrystal_I2CAdapter.h>
#include <input/KeyboardAdapter.h>
#include <renderer/CharacterDisplayRenderer.h>
#include <widget/WidgetBool.h>
#include <widget/WidgetList.h>
#include <widget/WidgetRange.h>

#define LCD_ROWS 2
#define LCD_COLS 16
#define LCD_ADDR 0x27

// Custom characters
byte plusMinus[8] = {B00000, B00100, B01110, B00100, B00000, B01110, B00000, B00000};  // ±
byte euro[8] = {B00111, B01000, B11110, B01000, B11110, B01000, B00111, B00000};       // €

std::vector<const char*> options = {"Buy", "Sell"};
std::vector<const char*> days = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
std::vector<char> pinChars = {'1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

MENU_SCREEN(
    mainScreen,
    mainItems,
    ITEM_WIDGET(
        "Auto",
        [](const uint8_t option, bool isAuto) { Serial.println(options[option]); Serial.println(isAuto); },
        WIDGET_LIST(options, 0, "%s", 0, true),
        WIDGET_BOOL(false, "Yes", "No", ",%s")),
    ITEM_WIDGET(
        "Price",
        [](int price) { Serial.println(price); },
        WIDGET_RANGE(10, 5, 0, 1000, "%.1d\003", 1)),
    ITEM_WIDGET(
        "Quantity",
        [](float quantity, int tolerance) { Serial.println(quantity); Serial.println(tolerance); },
        WIDGET_RANGE(1.0f, 0.1f, 0.1f, 100.0f, "%.1f", 0),
        WIDGET_RANGE(10, 1, 0, 100, "\002%d%%", 1)),
    ITEM_WIDGET(
        "Freq",
        [](int hour, int minute, const uint8_t day) { Serial.println(hour); Serial.println(minute); Serial.println(days[day]); },
        WIDGET_RANGE(0, 1, 0, 23, "%02d", 0, false),
        WIDGET_RANGE(0, 1, 0, 59, ":%02d", 0, false),
        WIDGET_LIST(days, 0, " on %s", 0, true)),
    ITEM_WIDGET(
        "Start",
        [](int day, int month, int year) { Serial.println(day); Serial.println(month); Serial.println(year); },
        WIDGET_RANGE(1, 1, 1, 31, "%02d", 0, true),
        WIDGET_RANGE(1, 1, 1, 12, "/%02d", 0, true),
        WIDGET_RANGE(2021, 1, 2020, 2050, "/%04d", 0, true)),
    ITEM_WIDGET(
        "Pin",
        [](const uint8_t d1, const uint8_t d2, const uint8_t d3, const uint8_t d4) { Serial.print(pinChars[d1]); Serial.print(pinChars[d2]); Serial.print(pinChars[d3]); Serial.println(pinChars[d4]); },
        WIDGET_LIST(pinChars, 2, "%c", 0, true),
        WIDGET_LIST(pinChars, 6, "%c", 0, true),
        WIDGET_LIST(pinChars, 10, "%c", 0, true),
        WIDGET_LIST(pinChars, 14, "%c", 0, true)));

LiquidCrystal_I2C lcd(LCD_ADDR, LCD_COLS, LCD_ROWS);
LiquidCrystal_I2CAdapter lcdAdapter(&lcd);
CharacterDisplayRenderer renderer(&lcdAdapter, LCD_COLS, LCD_ROWS);
LcdMenu menu(renderer);
KeyboardAdapter keyboard(&menu, &Serial);

void setup() {
    Serial.begin(9600);
    renderer.begin();
    menu.setScreen(mainScreen);
    lcd.createChar(2, plusMinus);
    lcd.createChar(3, euro);
}

void loop() {
    keyboard.observe();
}
