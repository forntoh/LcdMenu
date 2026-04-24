#include "Godmode.h"
#include <ArduinoUnitTests.h>
#include <ItemBool.h>
#include <ItemLabel.h>
#include <ItemList.h>
#include <ItemRange.h>
#include <ItemToggle.h>
#include <ItemValue.h>
#include <LcdMenu.h>
#include <MenuScreen.h>
#include <display/DisplayInterface.h>
#include <display/GraphicalDisplayInterface.h>
#include <renderer/GraphicalMenuItem.h>
#include <renderer/MenuRenderer.h>
#include <string.h>

#define LCD_ROWS 1
#define LCD_COLS 16

class CaptureDisplay : public DisplayInterface {
  public:
    std::string captured;
    void begin() override {}
    void clear() override { captured.clear(); }
    void show() override {}
    void hide() override {}
    void draw(uint8_t b) override { captured.push_back(static_cast<char>(b)); }
    void draw(const char* text) override {
        if (text) captured += text;
    }
    void setCursor(uint8_t, uint8_t) override {}
    void setBacklight(bool) override {}
};

class CaptureRenderer : public MenuRenderer {
  public:
    CaptureDisplay display;
    std::string lastText;
    std::string lastValue;
    CaptureRenderer() : MenuRenderer(&display, LCD_COLS, LCD_ROWS) {}
    void draw(uint8_t byte) override { display.draw(byte); }
    void drawItem(const char* text, const char* value, bool) override {
        lastText = text ? text : "";
        lastValue = value ? value : "";
    }
    void clearBlinker() override {}
    void drawBlinker() override {}
    uint8_t getEffectiveCols() const override { return maxCols; }
};

class GraphicalMeasureDisplay : public GraphicalDisplayInterface {
  public:
    static const uint8_t kCharWidth = 6;
    void begin() override {}
    void clear() override {}
    void show() override {}
    void hide() override {}
    void draw(uint8_t) override {}
    void draw(const char*) override {}
    void setCursor(uint8_t, uint8_t) override {}
    void setBacklight(bool) override {}
    void setFont(const uint8_t*) override {}
    uint8_t getDisplayWidth() const override { return 128; }
    uint8_t getDisplayHeight() const override { return 64; }
    uint8_t getFontWidth() const override { return kCharWidth; }
    uint8_t getFontHeight() const override { return 8; }
    uint8_t getTextWidth(const char* text) override {
        return text == NULL ? 0 : static_cast<uint8_t>(strlen(text) * kCharWidth);
    }
    void setDrawColor(uint8_t) override {}
    void clearBuffer() override {}
    void sendBuffer() override {}
    void drawBox(uint8_t, uint8_t, uint8_t, uint8_t) override {}
    void drawFrame(uint8_t, uint8_t, uint8_t, uint8_t) override {}
    void drawXbm(uint8_t, uint8_t, uint8_t, uint8_t, const uint8_t*) override {}
};

// clang-format off
float tracked = 0.0;
MENU_SCREEN(mainScreen, mainItems, ITEM_VALUE("Temp", tracked, "%.1f"));
// clang-format on

unittest(item_value_updates_after_poll) {
    CaptureRenderer renderer;
    LcdMenu menu(renderer);
    menu.setScreen(mainScreen);

    GODMODE()->micros = 200000;  // advance time
    menu.poll(100);
    assertEqual("0.0", renderer.lastValue.c_str());

    tracked = 42.5;
    GODMODE()->micros += 200000;  // advance time
    menu.poll(100);
    assertEqual("42.5", renderer.lastValue.c_str());
}

unittest(basic_and_label_expose_graphical_capability) {
    MenuItem* basic = ITEM_BASIC("Basic");
    ItemLabel* label = ITEM_LABEL("Label");

    assertTrue(basic->queryCapability(GraphicalMenuItem::capabilityId()) != NULL);
    assertTrue(label->queryCapability(GraphicalMenuItem::capabilityId()) != NULL);

    delete basic;
    delete label;
}

unittest(toggle_and_bool_report_graphical_toggle_state) {
    ItemToggle toggle("Power", "ON", "OFF", nullptr);
    const GraphicalMenuItem* toggleCapability = static_cast<const GraphicalMenuItem*>(toggle.queryCapability(GraphicalMenuItem::capabilityId()));
    assertTrue(toggleCapability != NULL);
    assertTrue(toggleCapability->hasGraphicalToggle());
    assertFalse(toggleCapability->graphicalToggleState());

    toggle.setIsOn(true);
    assertTrue(toggleCapability->graphicalToggleState());

    ItemBool<bool> boolItem("Enabled", true, "YES", "NO", "%s", 0, nullptr);
    const GraphicalMenuItem* boolCapability = static_cast<const GraphicalMenuItem*>(boolItem.queryCapability(GraphicalMenuItem::capabilityId()));
    assertTrue(boolCapability != NULL);
    assertTrue(boolCapability->hasGraphicalToggle());
    assertTrue(boolCapability->graphicalToggleState());
}

unittest(value_and_widget_items_measure_graphical_width) {
    GraphicalMeasureDisplay display;

    int value = 123;
    ItemValue<int> valueItem("Count", value, "%d");
    assertEqual((uint8_t)(3 * GraphicalMeasureDisplay::kCharWidth), valueItem.measureGraphicalValueWidth(&display));

    ItemBool<bool> boolItem("Enabled", false, "YES", "NO", "%s", 0, nullptr);
    assertEqual((uint8_t)(3 * GraphicalMeasureDisplay::kCharWidth), boolItem.measureGraphicalValueWidth(&display));
}

unittest(list_and_range_items_report_graphical_list_indicator) {
    std::vector<const char*> values = {"A", "B", "C"};
    ItemList<const char*, uint8_t>* listItem = ITEM_LIST("Mode", values, nullptr);
    ItemRange<int, int>* rangeItem = ITEM_RANGE("Speed", 3, 1, 0, 9, nullptr, "%d");

    const GraphicalMenuItem* listCapability = static_cast<const GraphicalMenuItem*>(listItem->queryCapability(GraphicalMenuItem::capabilityId()));
    const GraphicalMenuItem* rangeCapability = static_cast<const GraphicalMenuItem*>(rangeItem->queryCapability(GraphicalMenuItem::capabilityId()));

    assertTrue(listCapability != NULL);
    assertTrue(rangeCapability != NULL);
    assertTrue(listCapability->hasGraphicalListIndicator());
    assertTrue(rangeCapability->hasGraphicalListIndicator());

    delete listItem;
    delete rangeItem;
}

unittest_main()
