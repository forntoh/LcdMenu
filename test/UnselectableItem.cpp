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
    MenuItem* label = ITEM_LABEL("Title");
    MenuItem* first = ITEM_BASIC("First");
    MenuItem* second = ITEM_BASIC("Second");
    std::vector<MenuItem*> items = {label, first, second};
    MenuScreen screen(items);
    DummyRenderer renderer;
    screen.setCursor(&renderer, 0);
    assertEqual(1, screen.getCursor());
    screen.down(&renderer);
    assertEqual(2, screen.getCursor());
    screen.up(&renderer);
    assertEqual(1, screen.getCursor());
    delete label;
    delete first;
    delete second;
}

unittest(cursor_down_skips_multiple_labels) {
    MenuItem* a = ITEM_BASIC("A");
    MenuItem* l1 = ITEM_LABEL("Label1");
    MenuItem* l2 = ITEM_LABEL("Label2");
    MenuItem* b = ITEM_BASIC("B");
    std::vector<MenuItem*> items = {a, l1, l2, b};
    MenuScreen screen(items);
    DummyRenderer renderer;
    screen.setCursor(&renderer, 0);
    assertEqual(0, screen.getCursor());
    screen.down(&renderer);
    assertEqual(3, screen.getCursor());
    delete a;
    delete l1;
    delete l2;
    delete b;
}

unittest_main()
