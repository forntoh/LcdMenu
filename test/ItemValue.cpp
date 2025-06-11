#include <ArduinoUnitTests.h>
#include <LcdMenu.h>
#include <MenuScreen.h>
#include <ItemValue.h>
#include <display/DisplayInterface.h>
#include <renderer/MenuRenderer.h>
#include "Godmode.h"

#define LCD_ROWS 2
#define LCD_COLS 16

class CaptureDisplay : public DisplayInterface {
  public:
    std::string captured;
    void begin() override {}
    void clear() override { captured.clear(); }
    void show() override {}
    void hide() override {}
    void draw(uint8_t b) override { captured.push_back(static_cast<char>(b)); }
    void draw(const char* text) override { if (text) captured += text; }
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

// clang-format off
float tracked = 0.0;
MENU_SCREEN(mainScreen, mainItems, ITEM_VALUE("Temp", tracked, "%.1f"));
// clang-format on

unittest(item_value_updates_after_poll) {
    CaptureRenderer renderer;
    LcdMenu menu(renderer);
    menu.setScreen(mainScreen);

    GODMODE()->micros = 200000; // advance time
    menu.poll(100);
    assertEqual("0.0", renderer.lastValue.c_str());

    tracked = 42.5;
    GODMODE()->micros += 200000; // advance time
    menu.poll(100);
    assertEqual("42.5", renderer.lastValue.c_str());
}

unittest_main()
