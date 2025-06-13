#define protected public
#include <MenuScreen.h>
#undef protected
#include <ArduinoUnitTests.h>
#include <MenuItem.h>
#include <display/DisplayInterface.h>
#include <renderer/MenuRenderer.h>

class StubDisplay : public DisplayInterface {
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

class StubRenderer : public MenuRenderer {
  public:
    StubDisplay display;
    StubRenderer() : MenuRenderer(&display, 16, 2) {}

    void draw(uint8_t) override {}
    void drawItem(const char*, const char*, bool) override {}
    void clearBlinker() override {}
    void drawBlinker() override {}
    uint8_t getEffectiveCols() const override { return maxCols; }
};

class CountingItem : public MenuItem {
  public:
    uint8_t drawCount = 0;
    CountingItem(const char* t) : MenuItem(t) {}
    void draw(MenuRenderer*) override { drawCount++; }
};

unittest(menu_screen_dynamic_item_management) {
    MenuItem* i1 = ITEM_BASIC("One");
    MenuItem* i2 = ITEM_BASIC("Two");
    std::vector<MenuItem*> items = {i1, i2};
    MenuScreen screen(items);
    StubRenderer renderer;

    assertEqual((size_t)2, screen.size());
    assertEqual(i1, screen.getItemAt(0));

    MenuItem* i3 = ITEM_BASIC("Three");
    screen.addItem(i3);
    assertEqual((size_t)3, screen.size());
    assertEqual(i3, screen.getItemAt(2));
    screen.setCursor(&renderer, 2);
    assertEqual((uint8_t)2, screen.getCursor());

    MenuItem* i4 = ITEM_BASIC("Four");
    screen.addItemAt(1, i4);
    assertEqual((size_t)4, screen.size());
    assertEqual(i4, screen.getItemAt(1));
    screen.setCursor(&renderer, 3);
    assertEqual((uint8_t)3, screen.getCursor());

    screen.removeItemAt(1);
    assertEqual((size_t)3, screen.size());
    screen.setCursor(&renderer, screen.getCursor());
    assertEqual((uint8_t)2, screen.getCursor());
    assertEqual(i3, screen.getItemAt(2));
    delete i4;  // removed item no longer used

    screen.removeLastItem();
    assertEqual((size_t)2, screen.size());
    screen.setCursor(&renderer, screen.getCursor());
    assertEqual((uint8_t)1, screen.getCursor());
    assertEqual(i2, screen.getItemAt(1));
    delete i3;  // removed last item

    screen.clear();
    assertEqual((size_t)0, screen.size());
    screen.setCursor(&renderer, 0);
    assertEqual((uint8_t)0, screen.getCursor());

    delete i1;
    delete i2;
}

unittest(menu_screen_draw_clamps_view) {
    CountingItem* i1 = new CountingItem("One");
    CountingItem* i2 = new CountingItem("Two");
    CountingItem* i3 = new CountingItem("Three");
    CountingItem* i4 = new CountingItem("Four");
    CountingItem* i5 = new CountingItem("Five");
    std::vector<MenuItem*> items = {i1, i2, i3, i4, i5};
    MenuScreen screen(items);
    StubRenderer renderer;

    screen.setCursor(&renderer, 4);  // move cursor to last item, view becomes 3

    screen.removeLastItem();
    screen.removeLastItem();  // remove down to 3 items, view still 3

    screen.draw(&renderer);

    assertEqual((uint8_t)0, i1->drawCount);
    assertEqual((uint8_t)0, i2->drawCount);
    assertEqual((uint8_t)0, i3->drawCount);  // no items drawn but no crash

    delete i1;
    delete i2;
    delete i3;
    delete i4;
    delete i5;
}

unittest_main()
