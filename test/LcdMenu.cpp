#include <vector>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <string>
#include <type_traits>
#define protected public
#include <MenuScreen.h>
#undef protected
#include <ItemCommand.h>
#include <ItemInput.h>
#include <ItemToggle.h>
#include <display/DisplayInterface.h>
#include <renderer/MenuRenderer.h>

struct TestCase {
    const char* name;
    void (*func)();
};

static std::vector<TestCase> __tests;

#define unittest(name) \
    void name(); \
    struct __reg_##name { __reg_##name() { __tests.push_back({#name, name}); } } __reg_instance_##name; \
    void name()

template<typename A, typename B>
inline typename std::enable_if<
    std::is_convertible<A, const char*>::value && std::is_convertible<B, const char*>::value,
    bool>::type __eq(const A& a, const B& b) {
    return std::strcmp(static_cast<const char*>(a), static_cast<const char*>(b)) == 0;
}

template<typename A, typename B>
inline typename std::enable_if<
    !(std::is_convertible<A, const char*>::value && std::is_convertible<B, const char*>::value),
    bool>::type __eq(const A& a, const B& b) {
    return a == b;
}

#define assertTrue(cond) do { if(!(cond)) { \
    std::cerr << __FILE__ << ":" << __LINE__ << " assertTrue(" << #cond << ") failed" << std::endl; \
    std::exit(1); } } while(0)

#define assertEqual(expected, actual) do { auto _exp = (expected); auto _act = (actual); \
    if(!__eq(_exp, _act)) { \
        std::cerr << __FILE__ << ":" << __LINE__ << " assertEqual failed" << std::endl; \
        std::exit(1); } } while(0)

#define unittest_main() \
    int main() { \
        for(const auto& t : __tests) { \
            std::cout << t.name << "..." << std::endl; \
            t.func(); \
        } \
        std::cout << __tests.size() << " tests ran" << std::endl; \
        return 0; \
    }

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

unittest_main()
