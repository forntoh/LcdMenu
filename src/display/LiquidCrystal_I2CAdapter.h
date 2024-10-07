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
 * This class provides an interface to control LiquidCrystal_I2C displays using
 * the CharacterDisplayInterface. It includes methods for initializing the display,
 * creating custom characters, setting the backlight, positioning the cursor,
 * drawing text and characters, and managing a display timer.
 *
 * @note This class requires the LiquidCrystal_I2C library.
 *
 * @param lcd Pointer to a LiquidCrystal_I2C object that this adapter will interact with.
 */
class LiquidCrystal_I2CAdapter : public CharacterDisplayInterface {
  private:
    LiquidCrystal_I2C* lcd;

  public:
    LiquidCrystal_I2CAdapter(LiquidCrystal_I2C* lcd) : CharacterDisplayInterface(), lcd(lcd) {}

    void begin() override {
        lcd->init();
        lcd->clear();
        lcd->backlight();
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

    void show() override {
        lcd->display();
        lcd->backlight();
    }

    void hide() override {
        lcd->noDisplay();
        lcd->noBacklight();
    }

    void clear() override { lcd->clear(); }
};
