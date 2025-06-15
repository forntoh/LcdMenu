#pragma once

#include <U8g2lib.h>

#include "GraphicalDisplayInterface.h"

class U8g2DisplayAdapter : public GraphicalDisplayInterface {
  private:
    U8G2* u8g2;
    uint8_t cursorX = 0;
    uint8_t cursorY = 0;

  public:
    explicit U8g2DisplayAdapter(U8G2* u8g2) : u8g2(u8g2) {}

    void begin() override {
        u8g2->begin();
        u8g2->clearBuffer();
        u8g2->sendBuffer();
    }

    void clear() override {
        u8g2->clearBuffer();
        u8g2->sendBuffer();
    }

    void clearBuffer() override { u8g2->clearBuffer(); }

    void sendBuffer() override { u8g2->sendBuffer(); }

    void show() override { u8g2->setPowerSave(0); }

    void hide() override { u8g2->setPowerSave(1); }

    void draw(uint8_t byte) override {
        char c[2] = {static_cast<char>(byte), '\0'};
        u8g2->drawStr(cursorX, cursorY, c);
    }

    void draw(const char* text) override { u8g2->drawStr(cursorX, cursorY, text); }

    void setCursor(uint8_t col, uint8_t row) override {
        cursorX = col;
        cursorY = row;
    }

    void setBacklight(bool) override {}

    void setFont(const uint8_t* font) override { u8g2->setFont(font); }
    uint8_t getDisplayWidth() const override { return u8g2->getDisplayWidth(); }
    uint8_t getDisplayHeight() const override { return u8g2->getDisplayHeight(); }
    uint8_t getFontWidth() const override { return u8g2->getMaxCharWidth(); }
    uint8_t getFontHeight() const override { return u8g2->getMaxCharHeight(); }
    uint8_t getTextWidth(const char* text) override { return u8g2->getStrWidth(text); }
    void drawBox(uint8_t x, uint8_t y, uint8_t w, uint8_t h) override {
        u8g2->drawBox(x, y, w, h);
    }
    void drawFrame(uint8_t x, uint8_t y, uint8_t w, uint8_t h) override {
        u8g2->drawFrame(x, y, w, h);
    }
};
