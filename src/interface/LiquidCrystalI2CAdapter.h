#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <utils/constants.h>

#include "DisplayInterface.h"

class LiquidCrystalI2CAdapter : public DisplayInterface {
   private:
    LiquidCrystal_I2C lcd;

    uint8_t downArrow[8];
    uint8_t upArrow[8];

   public:
    uint8_t cursorIcon = 0x7E;      // →
    uint8_t editCursorIcon = 0x7F;  // ←
    uint16_t timeout = 10000;       // 10 seconds

    unsigned long startTime = 0;

    LiquidCrystalI2CAdapter(uint8_t lcd_Addr, uint8_t lcd_cols,
                            uint8_t lcd_rows)
        : lcd(lcd_Addr, lcd_cols, lcd_rows) {
        maxRows = lcd_rows;
        maxCols = lcd_cols;
        memcpy(upArrow, UP_ARROW, sizeof(UP_ARROW));
        memcpy(downArrow, DOWN_ARROW, sizeof(DOWN_ARROW));
    }

    void begin() override {
        lcd.init();
        lcd.clear();
        lcd.backlight();
        lcd.createChar(0, upArrow);
        lcd.createChar(1, downArrow);
        startTime = millis();
    }

    void clear() override { lcd.clear(); }

    void setCursor(uint8_t col, uint8_t row) override {
        startTime = millis();
        lcd.setCursor(col, row);
    }

    void drawCursor(bool isInEditMode) override {
        lcd.write(isInEditMode ? editCursorIcon : cursorIcon);
    }

    void drawDownIndicator() override { lcd.write(byte(1)); }

    void drawUpIndicator() override { lcd.write(byte(0)); }

    void print(const char* str) override { lcd.print(str); }

    void print(const char str) override { lcd.print(str); }

    void display() override { lcd.display(); }

    void blink() override { lcd.blink(); }

    void noBlink() override { lcd.noBlink(); }

    void updateTimer() {
        if (millis() == startTime + timeout) {
            lcd.noDisplay();
        }
    }
};
