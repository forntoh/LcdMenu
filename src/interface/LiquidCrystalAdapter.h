#pragma once

#include <Arduino.h>
#include <LiquidCrystal.h>
#include <utils/constants.h>
#include <utils/utils.h>

#include "DisplayInterface.h"

class LiquidCrystalAdapter : public DisplayInterface {
   private:
    uint8_t downArrow[8];
    uint8_t upArrow[8];
    unsigned long startTime = 0;
    LiquidCrystal lcd;

   public:
    LiquidCrystalAdapter(uint8_t rs, uint8_t en, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t lcd_cols, uint8_t lcd_rows): lcd(rs, en, d0, d1, d2, d3) {
        maxRows = lcd_rows;
        maxCols = lcd_cols;
        memcpy(upArrow, UP_ARROW, sizeof(UP_ARROW));
        memcpy(downArrow, DOWN_ARROW, sizeof(DOWN_ARROW));
    }

    void begin() override {
        lcd.begin(maxCols, maxRows);
        lcd.clear();
        lcd.createChar(0, upArrow);
        lcd.createChar(1, downArrow);
        startTime = millis();
    }

    void clear() override { lcd.clear(); }

    void clearCursor() override {
        lcd.setCursor(0, cursorRow);
        lcd.print(" ");
    }

    void drawCursor() override {
        restartTimer();
        lcd.setCursor(0, cursorRow);
        lcd.write(isEditModeEnabled ? EDIT_CURSOR_ICON : CURSOR_ICON);
    }

    void drawItem(uint8_t row, const char* text) override {
        restartTimer();
        lcd.setCursor(1, row);
        lcd.print(text);
        uint8_t spaces = maxCols - 2 - strlen(text);
        for (uint8_t i = 0; i < spaces; i++) {
            lcd.print(" ");
        }
    }

    void drawItem(uint8_t row, const char* text, char separator, char* value) override {
        restartTimer();
        lcd.setCursor(1, row);
        uint8_t size = strlen(text) + 1 + strlen(value);
        lcd.print(text);
        lcd.print(separator);
        lcd.print(value);
        uint8_t spaces = maxCols - 2 - size;
        for (uint8_t i = 0; i < spaces; i++) {
            lcd.print(" ");
        }
    }
    
    void clearBlinker() override {
        lcd.noBlink();
    }
    
    void drawBlinker() override {
        lcd.blink();
    }

    void resetBlinker(uint8_t blinkerPosition) {
        this->blinkerPosition = blinkerPosition;
        lcd.setCursor(blinkerPosition, cursorRow);
    }
    
    void restartTimer() override {
        this->startTime = millis();
        lcd.display();
    }

    bool drawChar(char c) override {
        //
        // draw the character without updating the menu item
        //
        lcd.setCursor(blinkerPosition, cursorRow);
        lcd.print(c);
        // resetBlinker(blinkerPosition);
        // Log
        printCmd(F("DRAW-CHAR"), c);
        return true;
    }

    void updateTimer() {
        if (millis() != startTime + DISPLAY_TIMEOUT) {
            return;
        }
        printCmd(F("TIMEOUT"));
        lcd.noDisplay();
    }

    void clearDownIndicator() {
        lcd.setCursor(maxCols - 1, maxRows - 1);
        lcd.print(" ");
    }

    void drawDownIndicator() override { 
        lcd.setCursor(maxCols - 1, maxRows - 1);
        lcd.write(byte(1));
    }

    void clearUpIndicator() {
        lcd.setCursor(maxCols - 1, 0);
        lcd.print(" ");
    }

    void drawUpIndicator() override { 
        lcd.setCursor(maxCols - 1, 0);
        lcd.write(byte(0)); 
    }

};
