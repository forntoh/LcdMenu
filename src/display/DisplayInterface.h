#ifndef DISPLAY_INTERFACE_H
#define DISPLAY_INTERFACE_H

#include <stdint.h>
#include <utils/constants.h>

/**
 * @class DisplayInterface
 * @brief An abstract base class for display interfaces.
 *
 * This class defines the interface for display operations. It provides
 * pure virtual functions that must be implemented by any derived class.
 */
class DisplayInterface {
  public:
    DisplayInterface() {}
    virtual void begin() = 0;
    virtual void clear() = 0;
    virtual void show() = 0;
    virtual void hide() = 0;
    virtual void draw(uint8_t byte) = 0;
    virtual void draw(const char* text) = 0;
    virtual void setCursor(uint8_t col, uint8_t row) = 0;
    virtual void setBacklight(bool enabled) = 0;
    virtual ~DisplayInterface() {}
};

#endif  // DISPLAY_INTERFACE_H
