#pragma once

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <utils/constants.h>
#include <utils/utils.h>

#include "CharacterDisplayInterface.h"

/**
 * @class LiquidCrystal_I2CAdapter
 * @brief Adapter class for interfacing with LiquidCrystal_I2C displays.
 *
 * This class provides an interface to control LiquidCrystal_I2C displays,
 * implementing the DisplayInterface. It includes methods for initializing
 * the display, drawing items, handling cursors, and managing backlight and
 * display states.
 *
 * @note This class assumes that the LiquidCrystal_I2C library is included and available.
 *
 * @param lcd Pointer to a LiquidCrystal_I2C object.
 * @param maxCols Maximum number of columns on the display.
 * @param maxRows Maximum number of rows on the display.
 */
class LiquidCrystal_I2CAdapter : public CharacterDisplayInterface {
  private:
    LiquidCrystal_I2C* lcd;
    unsigned long startTime = 0;

  public:
    LiquidCrystal_I2CAdapter(LiquidCrystal_I2C* lcd) : CharacterDisplayInterface(), lcd(lcd) {}

    void begin() override {
        lcd->init();
        lcd->clear();
        lcd->backlight();
        startTime = millis();
    }

    void createChar(uint8_t id, uint8_t* c) override {
        lcd->createChar(id, c);
    }

    void setBacklight(bool enabled) override {
        lcd->setBacklight(enabled);
    }

    void setCursor(uint8_t col, uint8_t row) override {
        lcd->setCursor(col, row);
    }

    void draw(const char* text) override {
        lcd->print(text);
    }

    void draw(uint8_t byte) override {
        lcd->write(byte);
    }

    void drawBlinker() override {
        lcd->blink();
    }

    void clearBlinker() override {
        lcd->noBlink();
    }

    void restartTimer() override {
        this->startTime = millis();
        lcd->display();
        lcd->backlight();
    }

    void clear() override { lcd->clear(); }

    void updateTimer() {
        if (millis() != startTime + DISPLAY_TIMEOUT) {
            return;
        }
        printLog(F("LiquidCrystal_I2CAdapter::timeout"));
        lcd->noDisplay();
        lcd->noBacklight();
    }
};
