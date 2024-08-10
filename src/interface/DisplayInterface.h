#ifndef DISPLAY_INTERFACE_H
#define DISPLAY_INTERFACE_H

#include <stdint.h>

class DisplayInterface {
   protected:
    uint8_t maxRows;
    uint8_t maxCols;

   public:
    virtual void begin() = 0;
    virtual void clear() = 0;
    virtual void setCursor(uint8_t col, uint8_t row) = 0;
    virtual void drawCursor(bool isInEditMode) = 0;
    virtual void drawDownIndicator() = 0;
    virtual void drawUpIndicator() = 0;
    virtual void print(const char* str) = 0;
    virtual void print(const char str) = 0;
    virtual void display() = 0;
    virtual void blink() = 0;
    virtual void noBlink() = 0;

    virtual ~DisplayInterface() {}

    uint8_t getMaxRows() const { return maxRows; }
    uint8_t getMaxCols() const { return maxCols; }
};

#endif  // DISPLAY_INTERFACE_H
