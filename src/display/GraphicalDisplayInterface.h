#pragma once

#include "DisplayInterface.h"

class GraphicalDisplayInterface : public DisplayInterface {
  public:
    virtual void setFont(const uint8_t* font) = 0;
    virtual uint8_t getDisplayWidth() const = 0;
    virtual uint8_t getDisplayHeight() const = 0;
    virtual uint8_t getFontWidth() const = 0;
    virtual uint8_t getFontHeight() const = 0;
    virtual void clearBuffer() = 0;
    virtual void sendBuffer() = 0;
    virtual void drawBox(uint8_t x, uint8_t y, uint8_t w, uint8_t h) = 0;
    virtual void drawFrame(uint8_t x, uint8_t y, uint8_t w, uint8_t h) = 0;
    bool isGraphical() const override { return true; }
};
