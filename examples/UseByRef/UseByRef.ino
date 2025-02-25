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

uint8_t hour = 0;
uint8_t day = 0;
bool toggle = false;

MENU_SCREEN(
    mainScreen,
    mainItems,
    ITEM_WIDGET(
        "List",
        [](const uint8_t value) { day = value; },
        WIDGET_LIST(days, 7, day, "%s", 0, true)),
    ITEM_WIDGET(
        "List-ref",
        [](const uint8_t value) { Serial.println(days[value]); },
        WIDGET_LIST(days, 7, Ref<uint8_t>(day), "%s", 0, true)),
    ITEM_WIDGET(
        "Bool-ref",
        [](const bool value) { Serial.println(value); },
        WIDGET_BOOL(Ref<bool>(toggle), "Yes", "No", "%s")),
    ITEM_WIDGET(
        "Bool",
        [](const bool value) { toggle = value; },
        WIDGET_BOOL(toggle, "Yes", "No", "%s")));

LiquidCrystal_I2C lcd(LCD_ADDR, LCD_COLS, LCD_ROWS);
LiquidCrystal_I2CAdapter lcdAdapter(&lcd);
CharacterDisplayRenderer renderer(&lcdAdapter, LCD_COLS, LCD_ROWS);
LcdMenu menu(renderer);
KeyboardAdapter keyboard(&menu, &Serial);

void setup() {
    Serial.begin(115200);
    renderer.begin();
    menu.setScreen(mainScreen);
}

unsigned long last = 0;

/**
 * This function is used to validate the values of the day and toggle status
 * which are ref values in the menu items. It prints the current day and toggle.
 *
 * localDay and localToggle are used here instead of directly using the values defined above
 * to ensure that the ref values are correctly updated in the menu items.
 *
 * Success Scenario:
 * - The day and toggle status printed to the Serial monitor change every second.
 *
 * Failure Scenario:
 * - The day and toggle status printed to the Serial monitor do not change every second.
 */
void logger() {
    uint8_t localDay = static_cast<BaseWidgetValue<uint8_t>*>(static_cast<ItemWidget<uint8_t>*>(mainItems[1])->getWidgetAt(0))->getValue();
    bool localToggle = static_cast<BaseWidgetValue<bool>*>(static_cast<ItemWidget<bool>*>(mainItems[2])->getWidgetAt(0))->getValue();
    Serial.print("Day: ");
    Serial.print(days[localDay]);
    Serial.print(", Toggle: ");
    Serial.println(localToggle ? "Yes" : "No");
}

void loop() {
    keyboard.observe();
    menu.poll();

    unsigned long now = millis();
    if (now - last > 1000) {
        if (!menu.getRenderer()->isInEditMode()) {
            hour++;
            hour %= 23;
            day++;
            day %= 7;
            toggle = !toggle;
            logger();
        }
        last = now;
    }
}