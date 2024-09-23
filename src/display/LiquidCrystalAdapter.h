#pragma once

#include "utils/liquidcrystal.h"
#include <Arduino.h>
#include <LiquidCrystal.h>
#include <utils/constants.h>
#include <utils/utils.h>

#include "DisplayInterface.h"

class LiquidCrystalAdapter : public DisplayInterface {
  private:
    LiquidCrystal* lcd;
    uint8_t downArrow[8];
    uint8_t upArrow[8];
    unsigned long startTime = 0;

  public:
    LiquidCrystalAdapter(LiquidCrystal* lcd, uint8_t maxCols, uint8_t maxRows) : DisplayInterface(maxCols, maxRows), lcd(lcd) {
        memcpy(upArrow, UP_ARROW, sizeof(UP_ARROW));
        memcpy(downArrow, DOWN_ARROW, sizeof(DOWN_ARROW));
    }

    void begin() override {
        lcd->begin(maxCols, maxRows);
        lcd->clear();
        lcd->createChar(0, upArrow);
        lcd->createChar(1, downArrow);
        startTime = millis();
    }

    void clear() override { lcd->clear(); }

    void setBacklight(bool enabled) override {
        // Unsupported
    }

    void clearCursor() override {
        lcd->setCursor(0, cursorRow);
        lcd->print(" ");
    }

    void drawCursor() override {
        restartTimer();
        lcd->setCursor(0, cursorRow);
        lcd->write(isEditModeEnabled ? EDIT_CURSOR_ICON : CURSOR_ICON);
    }

    void drawItem(uint8_t row, const char* text) override {
        restartTimer();
        lcd->setCursor(1, row);
        lcd->print(text);
        uint8_t spaces = maxCols - 2 - strlen(text);
        for (uint8_t i = 0; i < spaces; i++) {
            lcd->print(" ");
        }
    }

    void drawItem(uint8_t row, const char* text, char separator, char* value) override {
        restartTimer();
        lcd->setCursor(1, row);
        uint8_t size = strlen(text) + 1 + strlen(value);
        lcd->print(text);
        lcd->print(separator);
        lcd->print(value);
        uint8_t spaces = maxCols - 2 - size;
        for (uint8_t i = 0; i < spaces; i++) {
            lcd->print(" ");
        }
    }

    void clearBlinker() override {
        lcd->noBlink();
    }

    void drawBlinker() override {
        lcd->blink();
    }

    void resetBlinker(uint8_t blinkerPosition) {
        restartTimer();
        this->blinkerPosition = blinkerPosition;
        lcd->setCursor(blinkerPosition, cursorRow);
    }

    void restartTimer() override {
        this->startTime = millis();
        lcd->display();
    }

    bool drawChar(char c) override {
        //
        // draw the character without updating the menu item
        //
        lcd->setCursor(blinkerPosition, cursorRow);
        lcd->print(c);
        lcd->setCursor(blinkerPosition, cursorRow);  // Move back
        return true;
    }

    void updateTimer() {
        if (millis() != startTime + DISPLAY_TIMEOUT) {
            return;
        }
        printLog(F("TIMEOUT"));
        lcd->noDisplay();
    }

    void clearDownIndicator() {
        lcd->setCursor(maxCols - 1, maxRows - 1);
        lcd->print(" ");
    }

    void drawDownIndicator() override {
        lcd->setCursor(maxCols - 1, maxRows - 1);
        lcd->write(byte(1));
    }

    void clearUpIndicator() {
        lcd->setCursor(maxCols - 1, 0);
        lcd->print(" ");
    }

    void drawUpIndicator() override {
        lcd->setCursor(maxCols - 1, 0);
        lcd->write(byte(0));
    }
};
