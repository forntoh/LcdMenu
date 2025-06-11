#define protected public
#include <ItemInput.h>
#include <MenuScreen.h>
#undef protected
#include <ArduinoUnitTests.h>
#include <ItemCommand.h>
#include <ItemToggle.h>
#include <display/DisplayInterface.h>
#include <renderer/MenuRenderer.h>

#define LCD_ROWS 2
#define LCD_COLS 16

#define ITEM_MAIN_HEADER_INDEX 0
#define ITEM_INPUT_INDEX 0
#define ITEM_COMMAND_INDEX 3
#define ITEM_TOGGLE_INDEX 4
#define ITEM_LIST_INDEX 99

void commandCallback() {}
void toggleCallback(bool) {}

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
    StubRenderer() : MenuRenderer(&display, LCD_COLS, LCD_ROWS) {}

    void draw(uint8_t) override {}
    void drawItem(const char*, const char*, bool) override {}
    void clearBlinker() override {}
    void drawBlinker() override {}
    uint8_t getEffectiveCols() const override { return maxCols; }
};

class TrackingDisplay : public DisplayInterface {
  public:
    bool cleared = false;
    void begin() override {}
    void clear() override { cleared = true; }
    void show() override {}
    void hide() override {}
    void draw(uint8_t) override {}
    void draw(const char*) override {}
    void setCursor(uint8_t, uint8_t) override {}
    void setBacklight(bool) override {}
};

class TrackingRenderer : public MenuRenderer {
  public:
    TrackingDisplay display;
    bool itemDrawn = false;
    TrackingRenderer() : MenuRenderer(&display, LCD_COLS, LCD_ROWS) {}

    void draw(uint8_t) override {}
    void drawItem(const char*, const char*, bool) override { itemDrawn = true; }
    void clearBlinker() override {}
    void drawBlinker() override {}
    uint8_t getEffectiveCols() const override { return maxCols; }
};

// clang-format off
MENU_SCREEN(mainScreen, mainItems,
    ITEM_INPUT("Random", NULL),
    ITEM_INPUT("Connect", NULL),
    ITEM_BASIC("Settings"),
    ITEM_COMMAND("Backlight", commandCallback),
    ITEM_TOGGLE("Toggle", "ON", "OFF", toggleCallback),
    ITEM_BASIC("Blink random"));
// clang-format on

unittest(text_set_correctly) {
    assertEqual("Random", mainItems[ITEM_INPUT_INDEX]->getText());
}

unittest(text_off_on_for_item_toggle) {
    assertEqual("ON", (static_cast<ItemToggle*>(mainItems[ITEM_TOGGLE_INDEX]))->getTextOn());
    assertEqual("OFF", (static_cast<ItemToggle*>(mainItems[ITEM_TOGGLE_INDEX]))->getTextOff());
}

unittest(can_set_input_value) {
    char* expected = "TEST";
    assertEqual("", (static_cast<ItemInput*>(mainItems[ITEM_INPUT_INDEX]))->getValue());
    (static_cast<ItemInput*>(mainItems[ITEM_INPUT_INDEX]))->setValue(expected);
    assertEqual(expected, (static_cast<ItemInput*>(mainItems[ITEM_INPUT_INDEX]))->getValue());
}

unittest(cursor_clamped_when_out_of_range) {
    StubRenderer renderer;
    uint8_t outOfRange = 100;
    mainScreen->setCursor(&renderer, outOfRange);
    assertEqual(mainScreen->size() - 1, mainScreen->getCursor());
}

unittest(clear_command_empties_input_and_resets_cursor) {
    char value[] = "HELLO";
    ItemInput item("Name", value, NULL);
    StubRenderer renderer;
    LcdMenu menu(renderer);

    assertTrue(item.process(&menu, ENTER));
    assertTrue(item.process(&menu, CLEAR));

    assertEqual("", item.getValue());
    assertEqual((uint8_t)0, item.cursor);
    assertEqual((uint8_t)0, item.view);
    assertTrue(renderer.isInEditMode());
}

unittest(hide_disables_and_clears_display) {
    TrackingRenderer renderer;
    LcdMenu menu(renderer);
    menu.setScreen(mainScreen);
    renderer.display.cleared = false;
    menu.hide();
    assertFalse(menu.isEnabled());
    assertTrue(renderer.display.cleared);
}

unittest(show_enables_and_draws_active_screen) {
    TrackingRenderer renderer;
    LcdMenu menu(renderer);
    menu.setScreen(mainScreen);
    menu.hide();
    renderer.display.cleared = false;
    renderer.itemDrawn = false;
    menu.show();
    assertTrue(menu.isEnabled());
    assertTrue(renderer.display.cleared);
    assertTrue(renderer.itemDrawn);
}

unittest_main()
