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
  public:
    DisplayInterface() {}
    virtual void begin() = 0;
    virtual void clear() = 0;
    virtual void setBacklight(bool enabled) = 0;
    virtual void setCursor(uint8_t col, uint8_t row) = 0;
    virtual void draw(const char* text) = 0;
    virtual void draw(uint8_t byte) = 0;
    virtual void restartTimer() = 0;
    virtual ~DisplayInterface() {}
};

#endif  // DISPLAY_INTERFACE_H
