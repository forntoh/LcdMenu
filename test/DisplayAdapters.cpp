#include "stubs/LiquidCrystal.h"
#include "stubs/LCD_I2C.h"
#include "stubs/LiquidCrystal_I2C.h"
#include "stubs/SSD1803A_I2C.h"

#include <vector>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <string>
#include <type_traits>

#include <display/LCD_I2CAdapter.h>
#include <display/LiquidCrystalAdapter.h>
#include <display/LiquidCrystal_I2CAdapter.h>
#include <display/SSD1803A_I2CAdapter.h>

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
unittest(liquid_crystal_adapter_calls) {
    LiquidCrystal lcd;
    LiquidCrystalAdapter adapter(&lcd, 16, 2);
    adapter.begin();
    assertTrue(lcd.beginCalled);
    assertTrue(lcd.clearCalled);
    adapter.show();
    assertTrue(lcd.displayCalled);
    adapter.hide();
    assertTrue(lcd.noDisplayCalled);
    adapter.setCursor(3, 1);
    assertTrue(lcd.setCursorCalled);
    assertEqual(3, lcd.cursorCol);
    assertEqual(1, lcd.cursorRow);
    adapter.draw("ABC");
    assertTrue(lcd.printCalled);
    assertEqual("ABC", lcd.printed.c_str());
    adapter.draw((uint8_t)5);
    assertTrue(lcd.writeCalled);
    assertEqual((uint8_t)5, lcd.written);
    uint8_t map[8] = {0};
    adapter.createChar(1, map);
    assertTrue(lcd.createCharCalled);
    assertEqual((uint8_t)1, lcd.createCharId);
}

unittest(lcd_i2c_adapter_calls) {
    LCD_I2C lcd;
    LCD_I2CAdapter adapter(&lcd);
    adapter.begin();
    assertTrue(lcd.beginCalled);
    assertTrue(lcd.clearCalled);
    assertTrue(lcd.backlightCalled);
    adapter.show();
    assertTrue(lcd.displayCalled);
    assertTrue(lcd.backlightCalled);
    adapter.hide();
    assertTrue(lcd.noDisplayCalled);
    assertTrue(lcd.noBacklightCalled);
    adapter.setCursor(4, 0);
    assertTrue(lcd.setCursorCalled);
    assertEqual(4, lcd.cursorCol);
    assertEqual(0, lcd.cursorRow);
    adapter.draw("XYZ");
    assertTrue(lcd.printCalled);
    assertEqual("XYZ", lcd.printed.c_str());
    adapter.draw((uint8_t)2);
    assertTrue(lcd.writeCalled);
    assertEqual((uint8_t)2, lcd.written);
    uint8_t map[8] = {0};
    adapter.createChar(2, map);
    assertTrue(lcd.createCharCalled);
    assertEqual((uint8_t)2, lcd.createCharId);
}

unittest(liquid_crystal_i2c_adapter_calls) {
    LiquidCrystal_I2C lcd;
    LiquidCrystal_I2CAdapter adapter(&lcd);
    adapter.begin();
    assertTrue(lcd.initCalled);
    assertTrue(lcd.clearCalled);
    assertTrue(lcd.backlightCalled);
    adapter.show();
    assertTrue(lcd.displayCalled);
    assertTrue(lcd.backlightCalled);
    adapter.hide();
    assertTrue(lcd.noDisplayCalled);
    assertTrue(lcd.noBacklightCalled);
    adapter.setCursor(1, 1);
    assertTrue(lcd.setCursorCalled);
    assertEqual(1, lcd.cursorCol);
    assertEqual(1, lcd.cursorRow);
    adapter.draw("123");
    assertTrue(lcd.printCalled);
    assertEqual("123", lcd.printed.c_str());
    adapter.draw((uint8_t)7);
    assertTrue(lcd.writeCalled);
    assertEqual((uint8_t)7, lcd.written);
    uint8_t map[8] = {0};
    adapter.createChar(3, map);
    assertTrue(lcd.createCharCalled);
    assertEqual((uint8_t)3, lcd.createCharId);
}

unittest(ssd1803a_i2c_adapter_calls) {
    SSD1803A_I2C lcd;
    SSD1803A_I2CAdapter adapter(&lcd);
    adapter.begin();
    assertTrue(lcd.displayCalled);
    assertEqual(DISPLAY_ON, lcd.displayState);
    adapter.show();
    assertTrue(lcd.displayCalled);
    assertEqual(DISPLAY_ON, lcd.displayState);
    adapter.hide();
    assertEqual(DISPLAY_OFF, lcd.displayState);
    adapter.setCursor(2, 3);
    assertTrue(lcd.locateCalled);
    assertEqual(3, lcd.row); // row comes first in locate()
    assertEqual(2, lcd.col);
    adapter.draw("hello");
    assertTrue(lcd.printCalled);
    assertEqual("hello", lcd.printed.c_str());
    adapter.draw((uint8_t)9);
    assertTrue(lcd.writeCalled);
    assertEqual((uint8_t)9, lcd.written);
    uint8_t map[8] = {0};
    adapter.createChar(4, map);
    assertTrue(lcd.createCalled);
    assertEqual((uint8_t)4, lcd.createId);
}

unittest_main()
