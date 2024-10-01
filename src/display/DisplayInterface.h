#ifndef DISPLAY_INTERFACE_H
#define DISPLAY_INTERFACE_H

#include <stdint.h>
#include <utils/constants.h>

/**
 * @class DisplayInterface
 * @brief An abstract base class for display interfaces.
 *
 * This class provides a common interface for various display implementations.
 * It includes methods for managing display properties, cursor, blinker, and indicators.
 *
 * ```
 *    ┌── Cursor
 *    v            v──── Items
 *   ┌──┬──────────────────────────────────────┬──┐
 *   │  │ S O M E  T E X T                     │^ │
 *   │ >│ S O M E  T E X T                     │  │
 *   │  │ S O M E  T E X T                     │  │
 *   │  │ S O M E  T E X T : V A L █           │v │
 *   └──┴──────────────────────────────────────┴──┘
 *                       Blinker ──^            ^
 *                               Indicators ────┘
 * ```
 */
class DisplayInterface {
  protected:
    uint8_t maxCols;
    uint8_t maxRows;
    uint8_t cursorRow = 0;
    uint8_t blinkerPosition = 0;
    bool isEditModeEnabled = false;

  public:
    DisplayInterface(uint8_t maxCols, uint8_t maxRows) : maxCols(maxCols), maxRows(maxRows) {}
    virtual void begin() = 0;
    uint8_t getMaxRows() const { return maxRows; }
    uint8_t getMaxCols() const { return maxCols; }
    virtual void clear() = 0;
    virtual void setBacklight(bool enabled) = 0;
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
    virtual void resetBlinker(uint8_t blinkerPosition);
    uint8_t getBlinkerPosition() const { return blinkerPosition; }
    virtual bool drawChar(char c) = 0;
    // Indicators
    virtual void clearUpIndicator();
    virtual void drawUpIndicator();
    virtual void clearDownIndicator();
    virtual void drawDownIndicator();
    // Timer
    virtual void restartTimer();
    // Destructor
    virtual ~DisplayInterface() {}
};

#endif  // DISPLAY_INTERFACE_H
