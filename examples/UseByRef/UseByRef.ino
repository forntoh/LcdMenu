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

const char* days[] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};

int hour = 0;
size_t day = 0;
bool toggle = false;

MENU_SCREEN(
    mainScreen,
    mainItems,
    ITEM_WIDGET(
        "Range val",
        [](const int value) { hour = value; },
        WIDGET_RANGE(hour, 1, 0, 23, "%02d", 0, false)),
    ITEM_WIDGET(
        "Range ref",
        [](const Ref<int> value) { Serial.println(value.value); },
        WIDGET_RANGE_REF(hour, 1, 0, 23, "%02d", 0, false)),
    ITEM_WIDGET(
        "List val",
        [](const size_t value) { day = value; },
        WIDGET_LIST(days, 7, day, "%s", 0, true)),
    ITEM_WIDGET(
        "List ref",
        [](const Ref<size_t> value) {Serial.println(days[value.value]); },
        WIDGET_LIST_REF(days, 7, day, "%s", 0, true)),
    ITEM_WIDGET(
        "Bool val",
        [](const bool value) { toggle = value; },
        WIDGET_BOOL(toggle, "Yes", "No", "%s")),
    ITEM_WIDGET(
        "Bool ref",
        [](const Ref<bool> value) { Serial.println(value.value); },
        WIDGET_BOOL_REF(toggle, "Yes", "No", "%s"))
    );

LiquidCrystal_I2C lcd(LCD_ADDR, LCD_COLS, LCD_ROWS);
LiquidCrystal_I2CAdapter lcdAdapter(&lcd);
CharacterDisplayRenderer renderer(&lcdAdapter, LCD_COLS, LCD_ROWS);
LcdMenu menu(renderer);
KeyboardAdapter keyboard(&menu, &Serial);

void setup() {
    Serial.begin(9600);
    renderer.begin();
    menu.setScreen(mainScreen);
}

unsigned long last = 0;

void loop() {
    keyboard.observe();
    unsigned long now = millis();
    if (now - last > 1000) {
        if (!menu.getRenderer()->isInEditMode()) {
            hour++;
            hour %= 23;
            day++;
            day %= 7;
            toggle = !toggle;
            menu.refresh();
        }
        last = now;
    }
}
