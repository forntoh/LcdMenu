#define protected public
#include <MenuScreen.h>
#undef protected
#include <ArduinoUnitTests.h>
#include <ItemLabel.h>
#include <MenuItem.h>
#include <display/DisplayInterface.h>
#include <renderer/MenuRenderer.h>

class DummyDisplay : public DisplayInterface {
  public:
    void begin() override {}
    void clear() override {}
    void show() override {}
    void hide() override {}
    void draw(uint8_t) override {}
    void draw(const char*) override {}
    void setCursor(uint8_t, uint8_t) override {}
    void setBacklight(bool) override {}
};

class DummyRenderer : public MenuRenderer {
  public:
    DummyDisplay display;
    DummyRenderer() : MenuRenderer(&display, 16, 2) {}
    void draw(uint8_t) override {}
    void drawItem(const char*, const char*, bool) override {}
    void clearBlinker() override {}
    void drawBlinker() override {}
    uint8_t getEffectiveCols() const override { return maxCols; }
};

unittest(cursor_skips_unselectable_items) {
    std::vector<MenuItem*> items = {ITEM_LABEL("Title"), ITEM_BASIC("First"), ITEM_BASIC("Second")};
    MenuScreen screen(items);
    DummyRenderer renderer;
    screen.setCursor(&renderer, 0);
    assertEqual(1, screen.getCursor());
    screen.down(&renderer);
    assertEqual(2, screen.getCursor());
    screen.up(&renderer);
    assertEqual(1, screen.getCursor());
}

unittest(cursor_down_skips_multiple_labels) {
    std::vector<MenuItem*> items = {ITEM_BASIC("A"), ITEM_LABEL("Label1"), ITEM_LABEL("Label2"), ITEM_BASIC("B")};
    MenuScreen screen(items);
    DummyRenderer renderer;
    screen.setCursor(&renderer, 0);
    assertEqual(0, screen.getCursor());
    screen.down(&renderer);
    assertEqual(3, screen.getCursor());
}

unittest_main()
