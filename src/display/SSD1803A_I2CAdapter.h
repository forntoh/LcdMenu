#pragma once

#include <Arduino.h>
#include <SSD1803A_I2C.h>
#include <utils/constants.h>
#include <utils/utils.h>

#include "CharacterDisplayInterface.h"

/**
 * @class SSD1803A_I2CAdapter
 * @brief Adapter class for interfacing with LCD Display Vision EA DOGM204 (4x20),
 *        DOGS164 (4x16) and DOGS104 (4x10) with SDD1803A controller using the I2C interface.
 *
 * @note This class requires the SSD1803A_I2C library.
 *
 * @param lcd Pointer to a SSD1803A_I2C object that this adapter will interact with.
 */
class SSD1803A_I2CAdapter : public CharacterDisplayInterface {
  private:
    SSD1803A_I2C* lcd;

  public:
    SSD1803A_I2CAdapter(SSD1803A_I2C* lcd) : CharacterDisplayInterface(), lcd(lcd) {}

    void begin() override {
        show();
    }

    void createChar(uint8_t id, uint8_t* c) override {
        lcd->create(id, c);
    }

    void setBacklight(bool enabled) override {
        // NO OP
    }

    void setCursor(uint8_t col, uint8_t row) override {
        lcd->locate(row, col);
    }

    uint8_t draw(const char* text) override {
        lcd->print(text);
        return strlen(text);
    }

    uint8_t draw(uint8_t byte) override {
        lcd->write(byte);
        return 1;
    }

    void drawBlinker() override {
        lcd->display(BLINK_ON);
    }

    void clearBlinker() override {
        lcd->display(BLINK_OFF);
    }

    void show() override {
        lcd->display(DISPLAY_ON);
    }

    void hide() override {
        lcd->display(DISPLAY_OFF);
    }

    void clear() override { lcd->cls(); }
};
