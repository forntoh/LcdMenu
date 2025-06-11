#include <ArduinoUnitTests.h>
#include <display/LCD_I2CAdapter.h>
#include <display/LiquidCrystalAdapter.h>
#include <display/LiquidCrystal_I2CAdapter.h>
#include <display/SSD1803A_I2CAdapter.h>

// Stub driver for LiquidCrystal
class FakeLiquidCrystal {
  public:
    bool beginCalled = false;
    uint8_t beginCols = 0;
    uint8_t beginRows = 0;
    bool clearCalled = false;
    bool displayCalled = false;
    bool noDisplayCalled = false;
    bool blinkCalled = false;
    bool noBlinkCalled = false;
    bool setCursorCalled = false;
    uint8_t cursorCol = 0;
    uint8_t cursorRow = 0;
    bool printCalled = false;
    String printed;
    bool writeCalled = false;
    uint8_t written = 0;
    bool createCharCalled = false;
    uint8_t createCharId = 0;
    uint8_t* createCharPtr = nullptr;

    void begin(uint8_t c, uint8_t r) {
        beginCalled = true;
        beginCols = c;
        beginRows = r;
    }
    void clear() { clearCalled = true; }
    void createChar(uint8_t id, uint8_t* ptr) {
        createCharCalled = true;
        createCharId = id;
        createCharPtr = ptr;
    }
    void setCursor(uint8_t col, uint8_t row) {
        setCursorCalled = true;
        cursorCol = col;
        cursorRow = row;
    }
    size_t print(const char* text) {
        printCalled = true;
        printed = text;
        return strlen(text);
    }
    size_t write(uint8_t b) {
        writeCalled = true;
        written = b;
        return 1;
    }
    void blink() { blinkCalled = true; }
    void noBlink() { noBlinkCalled = true; }
    void display() { displayCalled = true; }
    void noDisplay() { noDisplayCalled = true; }
};

// Stub driver for LCD_I2C
class FakeLCD_I2C {
  public:
    bool beginCalled = false;
    bool clearCalled = false;
    bool backlightCalled = false;
    bool noBacklightCalled = false;
    bool displayCalled = false;
    bool noDisplayCalled = false;
    bool blinkCalled = false;
    bool noBlinkCalled = false;
    bool setCursorCalled = false;
    uint8_t cursorCol = 0;
    uint8_t cursorRow = 0;
    bool printCalled = false;
    String printed;
    bool writeCalled = false;
    uint8_t written = 0;
    bool createCharCalled = false;
    uint8_t createCharId = 0;
    uint8_t* createCharPtr = nullptr;

    void begin() { beginCalled = true; }
    void clear() { clearCalled = true; }
    void backlight() { backlightCalled = true; }
    void noBacklight() { noBacklightCalled = true; }
    void display() { displayCalled = true; }
    void noDisplay() { noDisplayCalled = true; }
    void blink() { blinkCalled = true; }
    void noBlink() { noBlinkCalled = true; }
    void setCursor(uint8_t col, uint8_t row) {
        setCursorCalled = true;
        cursorCol = col;
        cursorRow = row;
    }
    size_t print(const char* text) {
        printCalled = true;
        printed = text;
        return strlen(text);
    }
    size_t write(uint8_t b) {
        writeCalled = true;
        written = b;
        return 1;
    }
    void createChar(uint8_t id, uint8_t* ptr) {
        createCharCalled = true;
        createCharId = id;
        createCharPtr = ptr;
    }
};

// Stub driver for LiquidCrystal_I2C
class FakeLiquidCrystal_I2C {
  public:
    bool initCalled = false;
    bool clearCalled = false;
    bool backlightCalled = false;
    bool noBacklightCalled = false;
    bool displayCalled = false;
    bool noDisplayCalled = false;
    bool blinkCalled = false;
    bool noBlinkCalled = false;
    bool setCursorCalled = false;
    uint8_t cursorCol = 0;
    uint8_t cursorRow = 0;
    bool printCalled = false;
    String printed;
    bool writeCalled = false;
    uint8_t written = 0;
    bool createCharCalled = false;
    uint8_t createCharId = 0;
    uint8_t* createCharPtr = nullptr;

    void init() { initCalled = true; }
    void clear() { clearCalled = true; }
    void backlight() { backlightCalled = true; }
    void noBacklight() { noBacklightCalled = true; }
    void display() { displayCalled = true; }
    void noDisplay() { noDisplayCalled = true; }
    void blink() { blinkCalled = true; }
    void noBlink() { noBlinkCalled = true; }
    void setCursor(uint8_t col, uint8_t row) {
        setCursorCalled = true;
        cursorCol = col;
        cursorRow = row;
    }
    size_t print(const char* text) {
        printCalled = true;
        printed = text;
        return strlen(text);
    }
    size_t write(uint8_t b) {
        writeCalled = true;
        written = b;
        return 1;
    }
    void createChar(uint8_t id, uint8_t* ptr) {
        createCharCalled = true;
        createCharId = id;
        createCharPtr = ptr;
    }
    void setBacklight(uint8_t val) {
        if (val)
            backlightCalled = true;
        else
            noBacklightCalled = true;
    }
};

// Stub driver for SSD1803A_I2C
class FakeSSD1803A_I2C {
  public:
    bool clsCalled = false;
    bool displayCalled = false;
    int displayState = 0;
    bool locateCalled = false;
    uint8_t row = 0;
    uint8_t col = 0;
    bool printCalled = false;
    String printed;
    bool writeCalled = false;
    uint8_t written = 0;
    bool createCalled = false;
    uint8_t createId = 0;
    uint8_t* createPtr = nullptr;

    void cls() { clsCalled = true; }
    void display(int state) {
        displayCalled = true;
        displayState = state;
    }
    void locate(uint8_t r, uint8_t c) {
        locateCalled = true;
        row = r;
        col = c;
    }
    size_t print(const char* text) {
        printCalled = true;
        printed = text;
        return strlen(text);
    }
    size_t write(uint8_t b) {
        writeCalled = true;
        written = b;
        return 1;
    }
    void create(uint8_t id, uint8_t* ptr) {
        createCalled = true;
        createId = id;
        createPtr = ptr;
    }
};

unittest(liquid_crystal_adapter_calls) {
    FakeLiquidCrystal lcd;
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
    FakeLCD_I2C lcd;
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
    FakeLiquidCrystal_I2C lcd;
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
    FakeSSD1803A_I2C lcd;
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
    assertEqual(2, lcd.row);
    assertEqual(3, lcd.col);
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
