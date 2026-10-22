#include <ArduinoUnitTests.h>
#include <ItemRange.h>
#include <LcdMenu.h>
#include <MenuScreen.h>
#include <display/CharacterDisplayInterface.h>
#include <renderer/CharacterDisplayRenderer.h>
#include <string.h>

class CellDisplay : public CharacterDisplayInterface {
  public:
    char cells[2][17];
    uint8_t col = 0;
    uint8_t row = 0;
    bool blinking = false;

    CellDisplay() { clear(); }
    void begin() override {}
    void clear() override {
        for (uint8_t i = 0; i < 2; i++) {
            memset(cells[i], ' ', 16);
            cells[i][16] = '\0';
        }
    }
    void show() override {}
    void hide() override {}
    void setBacklight(bool) override {}
    void setCursor(uint8_t c, uint8_t r) override {
        col = c;
        row = r;
    }
    void draw(uint8_t b) override {
        if (row < 2 && col < 16) cells[row][col] = b;
        col++;
    }
    void draw(const char* text) override {
        while (*text)
            draw(static_cast<uint8_t>(*text++));
    }
    void createChar(uint8_t, uint8_t*) override {}
    void drawBlinker() override { blinking = true; }
    void clearBlinker() override { blinking = false; }
};

unittest(range_edit_shifts_long_labels_to_keep_value_and_cursor_visible) {
    const char* labels[] = {"Short", "1234567890123", "12345678901234", "12345678901234567890"};
    for (const char* label : labels) {
        CellDisplay display;
        CharacterDisplayRenderer renderer(&display, 16, 2, '>', '<');
        LcdMenu menu(renderer);
        MenuItem* item = ITEM_RANGE<int>(label, 99, 1, 0, 101, nullptr, "%d%%", 1);
        MenuItem* other = ITEM_BASIC("Other row");
        MenuScreen screen({item, other});
        renderer.begin();
        menu.setScreen(&screen);

        menu.process(ENTER);
        assertTrue(display.blinking);
        assertTrue(strstr(display.cells[0], ":99%") != NULL);
        assertEqual('9', display.cells[0][display.col]);
        assertEqual(0, display.row);
        assertTrue(strstr(display.cells[1], "Other row") != NULL);

        menu.process(UP);
        assertTrue(strstr(display.cells[0], ":100%") != NULL);
        assertEqual('0', display.cells[0][display.col]);

        menu.process(ENTER);
        assertFalse(display.blinking);
        assertEqual(0, renderer.viewShift);
        delete item;
        delete other;
    }
}

unittest(range_ref_edit_shifts_long_label_and_restores_value_on_cancel) {
    CellDisplay display;
    CharacterDisplayRenderer renderer(&display, 16, 2, '>', '<');
    LcdMenu menu(renderer);
    int value = 99;
    MenuItem* item = ITEM_RANGE_REF<int>("12345678901234", value, 1, 0, 101, nullptr, "%d%%", 1);
    MenuScreen screen({item});
    renderer.begin();
    menu.setScreen(&screen);

    menu.process(ENTER);
    assertEqual(4, renderer.viewShift);
    assertTrue(strcmp(display.cells[0], "<5678901234:99% ") == 0);
    assertEqual(13, display.col);
    assertTrue(display.blinking);

    menu.process(UP);
    assertEqual(100, value);
    assertTrue(strcmp(display.cells[0], "<678901234:100% ") == 0);
    assertEqual(13, display.col);

    menu.process(BACK);
    assertEqual(99, value);
    assertFalse(display.blinking);
    assertEqual(0, renderer.viewShift);
    delete item;
}

unittest_main()
