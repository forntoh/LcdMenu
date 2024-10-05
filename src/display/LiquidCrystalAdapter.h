#pragma once

#include <Arduino.h>
#include <LiquidCrystal.h>
#include <utils/constants.h>
#include <utils/utils.h>

#include "DisplayInterface.h"

/**
 * @class LiquidCrystalAdapter
 * @brief Adapter class for interfacing with LiquidCrystal displays.
 *
 * This class provides an implementation of the DisplayInterface for LiquidCrystal displays.
 * It includes methods for initializing the display, drawing items, handling cursors, and managing
 * custom characters such as up and down arrows.
 *
 * @note This class assumes that the LiquidCrystal library is included and available.
 *
 * @param lcd Pointer to a LiquidCrystal object.
 * @param maxCols Maximum number of columns on the display.
 * @param maxRows Maximum number of rows on the display.
 */
class LiquidCrystalAdapter : public DisplayInterface {
  private:
    LiquidCrystal* lcd;
    uint8_t maxCols;
    uint8_t maxRows;
    unsigned long startTime = 0;

  public:
    LiquidCrystalAdapter(
        LiquidCrystal* lcd,
        uint8_t maxCols,
        uint8_t maxRows) : DisplayInterface(), lcd(lcd), maxCols(maxCols), maxRows(maxRows) {}

    void begin() override {
        lcd->begin(maxCols, maxRows);
        lcd->clear();
        startTime = millis();
    }

    void createChar(uint8_t id, uint8_t* c) {
        lcd->createChar(id, c);
    }

    void clear() override { lcd->clear(); }

    void setBacklight(bool enabled) override {}

    void setCursor(uint8_t col, uint8_t row) override {
        lcd->setCursor(col, row);
    }

    void draw(const char* text) override {
        lcd->print(text);
    }

    void draw(uint8_t byte) override {
        lcd->write(byte);
    }

    void drawBlinker() {
        lcd->blink();
    }

    void clearBlinker() {
        lcd->noBlink();
    }

    void restartTimer() override {
        this->startTime = millis();
        lcd->display();
    }

    void updateTimer() {
        if (millis() != startTime + DISPLAY_TIMEOUT) {
            return;
        }
        printLog(F("LiquidCrystal_I2CAdapter::timeout"));
        lcd->noDisplay();
    }
};
