#include <ItemBack.h>
#include <ItemBool.h>
#include <ItemCommand.h>
#include <ItemInput.h>
#include <ItemInputCharset.h>
#include <ItemLabel.h>
#include <ItemList.h>
#include <ItemRange.h>
#include <ItemSubMenu.h>
#include <ItemToggle.h>
#include <ItemValue.h>
#include <ItemWidget.h>
#include <LcdMenu.h>
#include <MenuScreen.h>
#include <U8g2lib.h>
#include <display/U8g2DisplayAdapter.h>
#include <input/KeyboardAdapter.h>
#include <renderer/GraphicalDisplayRenderer.h>
#include <widget/WidgetBool.h>
#include <widget/WidgetList.h>
#include <widget/WidgetRange.h>

#include <stdio.h>
#include <string.h>
#include <vector>

extern MenuScreen* navigationScreen;
extern MenuScreen* nestedScreen;
extern MenuScreen* inputScreen;
extern MenuScreen* valuesScreen;
extern MenuScreen* rangeListScreen;
extern MenuScreen* widgetScreen;
extern MenuScreen* dynamicScreen;

char* cloneText(const char* text) {
    size_t len = strlen(text);
    char* out = new char[len + 1];
    memcpy(out, text, len + 1);
    return out;
}

void refreshMainScreen();
void jumpToValuesScreen();
void onUserInput(char* value);
void onNoteInput(char* value);
void onTagInput(char* value);
void clearInputValues();
void onServiceToggle(bool enabled);
void onServiceToggleBox(bool enabled);
void onRelayToggle(const Ref<bool> enabled);
void onAlarmToggle(const bool enabled);
void onAlarmToggleBox(const bool enabled);
void flipRelayExternally();
void onBrightnessChanged(const Ref<int> value);
void onVolumeChanged(const Ref<int> value);
void onThemeChanged(const uint8_t index);
void onProfileChanged(const Ref<uint8_t> index);
void nextProfileExternally();
void onScheduleWidget(int hour, int minute);
void onControlWidget(uint8_t modeIndex, bool enabled);
void onPidWidget(int kp, int ki, int kd);
void addDynamicItem();
void insertDynamicItem();
void removeDynamicAtHead();
void removeDynamicLast();
void clearDynamicItems();
void updateTelemetry();

char* userName = cloneText("ALICE");
char* note = cloneText("HELLO");
char* tag = cloneText("A1");
const char* inputCharset = " ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-_";

unsigned long uptimeSeconds = 0;
float temperatureC = 22.0f;
float voltage = 3.30f;
uint16_t rpm = 1200;

bool relayEnabled = false;
int brightness = 40;
int volume = 12;
uint8_t profileIndex = 1;

std::vector<const char*> themes = {"Classic", "Comfort", "Minimal", "Dense"};
std::vector<const char*> profiles = {"A", "B", "C", "D", "E"};
std::vector<const char*> modes = {"Auto", "Manual", "Sleep", "Off"};

uint8_t dynamicBaseCount = 0;
uint8_t dynamicCounter = 0;

// clang-format off
MENU_SCREEN(mainScreen, mainItems,
    ITEM_FONT(ITEM_LABEL("SSD1306 showcase"), u8g2_font_7x13B_tf),
    ITEM_SUBMENU("Navigation", navigationScreen),
    ITEM_SUBMENU("Input", inputScreen),
    ITEM_SUBMENU("Values / toggles", valuesScreen),
    ITEM_SUBMENU("Range / list", rangeListScreen),
    ITEM_SUBMENU("Widget combos", widgetScreen),
    ITEM_SUBMENU("Dynamic menu", dynamicScreen),
    ITEM_COMMAND("Refresh now", refreshMainScreen),
    ITEM_BASIC("This row is intentionally very long to test clipping and horizontal shifting"));

MENU_SCREEN(navigationScreen, navigationItems,
    ITEM_BACK(".. Back"),
    ITEM_LABEL("Submenu indicator"),
    ITEM_SUBMENU("Nested level", nestedScreen),
    ITEM_COMMAND("Jump to values", jumpToValuesScreen),
    ITEM_BASIC("Use BACK key or .. item to return"));

MENU_SCREEN(nestedScreen, nestedItems,
    ITEM_BACK(".. Back"),
    ITEM_BASIC("Nested screen"),
    ITEM_BASIC("Submenu arrow should be visible"));

MENU_SCREEN(inputScreen, inputItems,
    ITEM_BACK(".. Back"),
    ITEM_LABEL("Input + charset"),
    ITEM_INPUT("User", userName, onUserInput),
    ITEM_INPUT("Note", note, onNoteInput),
    ITEM_INPUT_CHARSET("Tag", tag, inputCharset, onTagInput),
    ITEM_COMMAND("Clear inputs", clearInputValues),
    ITEM_BASIC("Try arrows, Enter, Delete"));

MENU_SCREEN(valuesScreen, valuesItems,
    ITEM_BACK(".. Back"),
    ITEM_FONT(ITEM_LABEL("Value text and boxes"), u8g2_font_7x13B_tf),
    ITEM_VALUE("Uptime", uptimeSeconds, "%lus"),
    ITEM_VALUE("Temp", temperatureC, "%.1fC"),
    ITEM_VALUE("Voltage", voltage, "%.2fV"),
    ITEM_VALUE("RPM", rpm, "%u"),
    ITEM_LABEL("Text values"),
    ITEM_TOGGLE("Service", "ON", "OFF", onServiceToggle),
    ITEM_BOOL_REF("Relay", relayEnabled, "ON", "OFF", onRelayToggle),
    ITEM_BOOL("Alarm", false, "ARM", "SAFE", onAlarmToggle),
    ITEM_LABEL("Checkbox values"),
    ITEM_TOGGLE("Service box", nullptr, nullptr, onServiceToggleBox),
    ITEM_BOOL("Alarm box", false, nullptr, nullptr, onAlarmToggleBox),
    ITEM_COMMAND("Flip relay externally", flipRelayExternally));

MENU_SCREEN(rangeListScreen, rangeListItems,
    ITEM_BACK(".. Back"),
    ITEM_LABEL("Range / list"),
    ITEM_RANGE_REF("Brightness", brightness, 5, 0, 100, onBrightnessChanged, "%d%%", 0, true),
    ITEM_RANGE_REF("Volume", volume, 1, 0, 30, onVolumeChanged, "%d", 0, true),
    ITEM_LIST("Theme", themes, onThemeChanged, 0, "%s", 0, true),
    ITEM_LIST_REF("Profile", profiles, onProfileChanged, profileIndex, "%s", 0, true),
    ITEM_COMMAND("Next profile external", nextProfileExternally));

MENU_SCREEN(widgetScreen, widgetItems,
    ITEM_BACK(".. Back"),
    ITEM_FONT(ITEM_LABEL("Multi-widget edit"), u8g2_font_7x13B_tf),
    ITEM_WIDGET(
        "Schedule",
        onScheduleWidget,
        WIDGET_RANGE(8, 1, 0, 23, "%02d", 0, true),
        WIDGET_RANGE(30, 5, 0, 55, ":%02d", 0, true)),
    ITEM_WIDGET(
        "Control",
        onControlWidget,
        WIDGET_LIST(modes, 0, "%s", 0, true),
        WIDGET_BOOL(false, "ON", "OFF", " %s")),
    ITEM_WIDGET(
        "PID",
        onPidWidget,
        WIDGET_RANGE(10, 1, 0, 99, "P:%02d", 0, true),
        WIDGET_RANGE(5, 1, 0, 99, " I:%02d", 0, true),
        WIDGET_RANGE(2, 1, 0, 99, " D:%02d", 0, true)),
    ITEM_BASIC("List indicator should be visible"));

MENU_SCREEN(dynamicScreen, dynamicItems,
    ITEM_BACK(".. Back"),
    ITEM_FONT(ITEM_LABEL("Runtime mutations"), u8g2_font_7x13B_tf),
    ITEM_COMMAND("Add item", addDynamicItem),
    ITEM_COMMAND("Insert at head", insertDynamicItem),
    ITEM_COMMAND("Remove first added", removeDynamicAtHead),
    ITEM_COMMAND("Remove last added", removeDynamicLast),
    ITEM_COMMAND("Clear all added", clearDynamicItems));
// clang-format on

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);
U8g2DisplayAdapter display(&u8g2);
GraphicalDisplayRenderer renderer(&display, u8g2_font_6x10_tf);
LcdMenu menu(renderer);
KeyboardAdapter keyboard(&menu, &Serial);

void setup() {
    Serial.begin(115200);

    renderer.begin();
    renderer.setItemFont(navigationItems[1], u8g2_font_7x13B_tf);
    renderer.setItemFont(inputItems[1], u8g2_font_7x13B_tf);
    renderer.setItemFont(rangeListItems[1], u8g2_font_7x13B_tf);

    dynamicBaseCount = dynamicScreen->size();

    menu.setScreen(mainScreen);

    Serial.println(F("SSD1306 showcase ready"));
    Serial.println(F("Controls: arrows, Enter, Esc, Delete, Backspace"));
}

void loop() {
    keyboard.observe();
    updateTelemetry();
    menu.poll(250);
}

void refreshMainScreen() {
    menu.refresh();
}

void jumpToValuesScreen() {
    menu.setScreen(valuesScreen);
}

void onUserInput(char* value) {
    Serial.print(F("User: "));
    Serial.println(value);
}

void onNoteInput(char* value) {
    Serial.print(F("Note: "));
    Serial.println(value);
}

void onTagInput(char* value) {
    Serial.print(F("Tag: "));
    Serial.println(value);
}

void clearInputValues() {
    ItemInput* user = static_cast<ItemInput*>(inputItems[2]);
    ItemInput* noteInput = static_cast<ItemInput*>(inputItems[3]);
    ItemInput* tagInput = static_cast<ItemInput*>(inputItems[4]);

    char* oldUser = user->getValue();
    char* oldNote = noteInput->getValue();
    char* oldTag = tagInput->getValue();

    user->setValue(cloneText(""));
    noteInput->setValue(cloneText(""));
    tagInput->setValue(cloneText(""));

    delete[] oldUser;
    delete[] oldNote;
    delete[] oldTag;

    menu.refresh();
}

void onServiceToggle(bool enabled) {
    Serial.print(F("Service: "));
    Serial.println(enabled ? F("ON") : F("OFF"));
}

void onServiceToggleBox(bool enabled) {
    Serial.print(F("Service box: "));
    Serial.println(enabled ? F("ON") : F("OFF"));
}

void onRelayToggle(const Ref<bool> enabled) {
    Serial.print(F("Relay: "));
    Serial.println(static_cast<bool>(enabled) ? F("ON") : F("OFF"));
}

void onAlarmToggle(const bool enabled) {
    Serial.print(F("Alarm: "));
    Serial.println(enabled ? F("ARM") : F("SAFE"));
}

void onAlarmToggleBox(const bool enabled) {
    Serial.print(F("Alarm box: "));
    Serial.println(enabled ? F("ON") : F("OFF"));
}

void flipRelayExternally() {
    relayEnabled = !relayEnabled;
    menu.refresh();
}

void onBrightnessChanged(const Ref<int> value) {
    Serial.print(F("Brightness: "));
    Serial.println(static_cast<int>(value));
}

void onVolumeChanged(const Ref<int> value) {
    Serial.print(F("Volume: "));
    Serial.println(static_cast<int>(value));
}

void onThemeChanged(const uint8_t index) {
    if (index < themes.size()) {
        Serial.print(F("Theme: "));
        Serial.println(themes[index]);
    }
}

void onProfileChanged(const Ref<uint8_t> index) {
    uint8_t i = static_cast<uint8_t>(index);
    if (i < profiles.size()) {
        Serial.print(F("Profile: "));
        Serial.println(profiles[i]);
    }
}

void nextProfileExternally() {
    profileIndex = static_cast<uint8_t>((profileIndex + 1) % profiles.size());
    menu.refresh();
}

void onScheduleWidget(int hour, int minute) {
    Serial.print(F("Schedule: "));
    if (hour < 10) {
        Serial.print('0');
    }
    Serial.print(hour);
    Serial.print(':');
    if (minute < 10) {
        Serial.print('0');
    }
    Serial.println(minute);
}

void onControlWidget(uint8_t modeIndex, bool enabled) {
    if (modeIndex < modes.size()) {
        Serial.print(F("Mode: "));
        Serial.print(modes[modeIndex]);
        Serial.print(F(" / Enabled: "));
        Serial.println(enabled ? F("ON") : F("OFF"));
    }
}

void onPidWidget(int kp, int ki, int kd) {
    Serial.print(F("PID: "));
    Serial.print(kp);
    Serial.print(' ');
    Serial.print(ki);
    Serial.print(' ');
    Serial.println(kd);
}

void addDynamicItem() {
    char text[24];
    snprintf(text, sizeof(text), "Added #%u", dynamicCounter++);
    dynamicScreen->addItem(ITEM_BASIC(cloneText(text)));
    menu.refresh();
}

void insertDynamicItem() {
    char text[24];
    snprintf(text, sizeof(text), "Inserted #%u", dynamicCounter++);
    dynamicScreen->addItemAt(dynamicBaseCount, ITEM_BASIC(cloneText(text)));
    menu.refresh();
}

void removeDynamicAtHead() {
    if (dynamicScreen->size() <= dynamicBaseCount) {
        return;
    }
    dynamicScreen->removeItemAt(dynamicBaseCount);
    menu.refresh();
}

void removeDynamicLast() {
    if (dynamicScreen->size() <= dynamicBaseCount) {
        return;
    }
    dynamicScreen->removeLastItem();
    menu.refresh();
}

void clearDynamicItems() {
    while (dynamicScreen->size() > dynamicBaseCount) {
        dynamicScreen->removeLastItem();
    }
    menu.refresh();
}

void updateTelemetry() {
    static unsigned long lastTick = 0;
    static int8_t temperatureDirection = 1;

    unsigned long now = millis();
    if (now - lastTick < 1000) {
        return;
    }
    lastTick = now;

    uptimeSeconds++;
    temperatureC += 0.2f * temperatureDirection;
    if (temperatureC > 28.0f || temperatureC < 21.0f) {
        temperatureDirection = -temperatureDirection;
    }
    voltage = 3.20f + 0.01f * (uptimeSeconds % 20);
    rpm = 1100 + (uptimeSeconds % 16) * 40;
}
