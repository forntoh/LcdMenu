#include <ArduinoUnitTests.h>
#include <display/LCD_I2CAdapter.h>
#include <display/LiquidCrystalAdapter.h>
#include <display/LiquidCrystal_I2CAdapter.h>
#include <display/SSD1803A_I2CAdapter.h>

// Include stubbed driver headers
#include "stubs/LiquidCrystal.h"
#include "stubs/LCD_I2C.h"
#include "stubs/LiquidCrystal_I2C.h"
#include "stubs/SSD1803A_I2C.h"

// The stub classes are defined in the headers above. They emulate the behaviour
// of the real display libraries used by the adapters.



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
