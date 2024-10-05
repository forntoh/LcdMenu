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
 * This class provides an interface to control LiquidCrystal displays using
 * the CharacterDisplayInterface. It includes methods for initializing the display,
 * creating custom characters, setting the backlight, positioning the cursor,
 * drawing text and characters, and managing a display timer.
 *
 * @note This class requires the LiquidCrystal library.
 *
 * @param lcd Pointer to a LiquidCrystal object that this adapter will interact with.
 * @param maxCols The maximum number of columns on the display.
 * @param maxRows The maximum number of rows on the display.
 */
class LiquidCrystalAdapter : public DisplayInterface {
  private:
    LiquidCrystal* lcd;
    uint8_t maxCols;
    uint8_t maxRows;

  public:
    LiquidCrystalAdapter(
        LiquidCrystal* lcd,
        uint8_t maxCols,
        uint8_t maxRows) : DisplayInterface(), lcd(lcd), maxCols(maxCols), maxRows(maxRows) {}

    void begin() override {
        lcd->begin(maxCols, maxRows);
        lcd->clear();
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

    void show() override {
        lcd->display();
    }

    void hide() override {
        lcd->noDisplay();
    }
};
