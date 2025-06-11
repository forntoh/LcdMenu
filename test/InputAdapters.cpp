#include <ArduinoUnitTests.h>
#include <input/ButtonAdapter.h>
#include <input/AnalogButtonAdapter.h>
#include <input/KeyboardAdapter.h>
#include <input/SimpleRotaryAdapter.h>
#include <display/DisplayInterface.h>
#include <renderer/MenuRenderer.h>
#include <MenuScreen.h>
#include <MenuItem.h>
#include <queue>

#define LCD_ROWS 2
#define LCD_COLS 16

// --------------------------------------------------
// Fake hardware helpers
// --------------------------------------------------

class FakeButton {
public:
    bool state;
    FakeButton(bool pressed = false) : state(pressed) {}
    bool pressed() {
        bool ret = state;
        state = false;
        return ret;
    }
};

static int16_t fakeAnalogValue = ButtonConfig::MAX_VALUE;
int16_t analogRead(uint8_t) { return fakeAnalogValue; }

static unsigned long fakeMillis = 0;
unsigned long millis() { return fakeMillis; }

class FakeStream : public Stream {
    std::queue<unsigned char> data;
public:
    FakeStream(std::initializer_list<unsigned char> seq) {
        for (auto b : seq) data.push(b);
    }
    int available() override { return !data.empty(); }
    int read() override {
        if (data.empty()) return -1;
        unsigned char b = data.front();
        data.pop();
        return b;
    }
    int peek() override { return data.empty() ? -1 : data.front(); }
    size_t write(uint8_t) override { return 1; }
};

class FakeRotary {
    std::queue<uint8_t> rotations;
    std::queue<uint8_t> pushes;
public:
    FakeRotary(const std::vector<uint8_t>& r = {}, const std::vector<uint8_t>& p = {}) {
        for (auto v : r) rotations.push(v);
        for (auto v : p) pushes.push(v);
    }
    uint8_t rotate() {
        if (rotations.empty()) return 0;
        uint8_t v = rotations.front();
        rotations.pop();
        return v;
    }
    uint8_t pushType(int) {
        if (pushes.empty()) return 0;
        uint8_t v = pushes.front();
        pushes.pop();
        return v;
    }
};

// --------------------------------------------------
// Minimal renderer/display for tests
// --------------------------------------------------

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
    StubRenderer() : MenuRenderer(&display, LCD_COLS, LCD_ROWS) {
        setEditMode(false);
    }
    void draw(uint8_t) override {}
    void drawItem(const char*, const char*, bool) override {}
    void clearBlinker() override {}
    void drawBlinker() override {}
    uint8_t getEffectiveCols() const override { return maxCols; }
};

class CaptureItem : public MenuItem {
  public:
    unsigned char lastCommand = 0;
    CaptureItem() : MenuItem("cap") {}
    bool process(LcdMenu*, const unsigned char command) override {
        lastCommand = command;
        return true;
    }
};

struct MenuFixture {
    StubRenderer renderer;
    CaptureItem item;
    std::vector<MenuItem*> items;
    MenuScreen screen;
    LcdMenu menu;
    MenuFixture() : items{&item}, screen(items), menu(renderer) {
        menu.setScreen(&screen);
    }
};

// --------------------------------------------------
// Tests
// --------------------------------------------------

unittest(button_adapter_sends_command) {
    MenuFixture f;
    FakeButton btn(true);
    ButtonAdapter adapter(&f.menu, &btn, ENTER);
    adapter.observe();
    assertEqual(ENTER, f.item.lastCommand);
}

unittest(analog_button_debounce) {
    MenuFixture f;
    AnalogButtonAdapter adapter(&f.menu, 0, 500, 10, RIGHT);
    fakeAnalogValue = 500;
    fakeMillis = 0;
    adapter.observe();
    assertEqual(RIGHT, f.item.lastCommand);
    f.item.lastCommand = 0;
    fakeMillis += 100; // within debounce window
    adapter.observe();
    assertEqual((unsigned char)0, f.item.lastCommand);
    fakeMillis += 400; // beyond debounce window
    adapter.observe();
    assertEqual(RIGHT, f.item.lastCommand);
}

unittest(keyboard_adapter_right_arrow) {
    MenuFixture f;
    FakeStream stream({ESC, '[', 'C'});
    KeyboardAdapter adapter(&f.menu, &stream);
    adapter.observe(); // ESC
    adapter.observe(); // [
    adapter.observe(); // C -> RIGHT
    assertEqual(RIGHT, f.item.lastCommand);
}

unittest(simple_rotary_double_press_backspace) {
    MenuFixture f;
    FakeRotary rot({}, {1,1});
    SimpleRotaryAdapter adapter(&f.menu, &rot);
    fakeMillis = 0;
    adapter.observe(); // first short press
    assertEqual((unsigned char)0, f.item.lastCommand);
    fakeMillis += 100; // < threshold
    adapter.observe(); // second short press within threshold
    assertEqual(BACKSPACE, f.item.lastCommand);
}

unittest(simple_rotary_single_press_enter) {
    MenuFixture f;
    FakeRotary rot({}, {1});
    SimpleRotaryAdapter adapter(&f.menu, &rot);
    fakeMillis = 0;
    adapter.observe(); // set pending
    fakeMillis += DOUBLE_PRESS_THRESHOLD + 1;
    adapter.observe(); // should trigger ENTER
    assertEqual(ENTER, f.item.lastCommand);
}

unittest(simple_rotary_long_press_back) {
    MenuFixture f;
    FakeRotary rot({}, {2});
    SimpleRotaryAdapter adapter(&f.menu, &rot);
    fakeMillis = 0;
    adapter.observe();
    assertEqual(BACK, f.item.lastCommand);
}

unittest(simple_rotary_rotation) {
    MenuFixture f;
    FakeRotary rot({1,2}, {});
    SimpleRotaryAdapter adapter(&f.menu, &rot);
    fakeMillis = 0;
    adapter.observe();
    assertEqual(DOWN, f.item.lastCommand);
    f.item.lastCommand = 0;
    adapter.observe();
    assertEqual(UP, f.item.lastCommand);
}

unittest_main()
