#ifndef DISPLAY_INTERFACE_H
#define DISPLAY_INTERFACE_H

#include <stdint.h>
#include <utils/constants.h>

/**
 *    +-- Cursor
 *    v            v---- Items
 *   +--+--------------------------------------+--+
 *   |  | S O M E  T E X T                     |^ |
 *   |->| S O M E  T E X T                     |  |
 *   |  | S O M E  T E X T                     |  |
 *   |  | S O M E  T E X T                     |v |
 *   +--+--------------------------------------+--+
 *                               Indicators ----^
 * 
 */
class DisplayInterface {
   protected:
    uint8_t maxRows;
    uint8_t maxCols;
    uint8_t cursorRow = 0;
    uint8_t blinkerPosition;
    bool isEditModeEnabled = false;

   public:
    virtual void begin() = 0;
    uint8_t getMaxRows() const { return maxRows; }
    uint8_t getMaxCols() const { return maxCols; }
    virtual void clear() = 0;
    // Items
    virtual void drawItem(uint8_t row, const char* text);
    virtual void drawItem(uint8_t row, const char* text, char separator, char* value);
    // Cursor
    virtual void clearCursor();
    virtual void drawCursor();
    virtual void moveCursor(uint8_t newCursorRow) {
        if (cursorRow == newCursorRow) {
            return;
        }
        clearCursor();
        cursorRow = newCursorRow;
        drawCursor();
    }
    void setEditModeEnabled(bool enabled) {
        if (isEditModeEnabled == enabled) {
            return;
        }
        isEditModeEnabled = enabled;
        drawCursor();
    }
    bool getEditModeEnabled() { return isEditModeEnabled; }
    uint8_t getCursorRow() const { return cursorRow; }
    // Blinker
    virtual void clearBlinker();
    virtual void drawBlinker();
    virtual void restartTimer();
    virtual void resetBlinker(uint8_t blinkerPosition);
    uint8_t getBlinkerCol() const { return blinkerPosition; }
    virtual bool drawChar(char c) = 0;
    // Indicators
    virtual void clearUpIndicator();
    virtual void drawUpIndicator();
    virtual void clearDownIndicator();
    virtual void drawDownIndicator();
    virtual ~DisplayInterface() {}
};

#endif  // DISPLAY_INTERFACE_H
