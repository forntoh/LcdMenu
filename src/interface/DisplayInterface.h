#ifndef DISPLAY_INTERFACE_H
#define DISPLAY_INTERFACE_H

#include <stdint.h>
#include "Print.h"

class DisplayInterface {
   protected:
    uint8_t maxRows;
    uint8_t maxCols;
    uint8_t cursorPosition;
    bool isEditModeEnabled;

   public:
    uint8_t blinkerPosition;

    virtual void begin() = 0;

    virtual void update(uint8_t cursorPosition, uint8_t top, uint8_t bottom) = 0;
    virtual void drawCursor(bool blink) = 0;
#ifdef ItemInput_H
    virtual void resetBlinker() = 0;
    virtual bool drawChar(char c) = 0;
#endif
    virtual void clear() = 0;

    virtual ~DisplayInterface() {}

    uint8_t getMaxRows() const { return maxRows; }
    uint8_t getMaxCols() const { return maxCols; }

    virtual void setEditModeEnabled(bool isEnabled) { isEditModeEnabled = isEnabled; }
    bool getEditModeEnabled() { return isEditModeEnabled; }
    virtual Print* getPrint();
    virtual void setCursor(uint8_t col, uint8_t row);
    virtual void drawDownIndicator(uint8_t col, uint8_t row);
    virtual void drawUpIndicator(uint8_t col, uint8_t row);
};

#endif  // DISPLAY_INTERFACE_H
