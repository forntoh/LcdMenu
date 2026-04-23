#pragma once

#include <stdint.h>

class GraphicalDisplayInterface;

/**
 * @brief Optional capabilities for items rendered on graphical displays.
 */
class GraphicalMenuItem {
  public:
    static uint8_t capabilityId() { return 1; }

    virtual ~GraphicalMenuItem() {}

    virtual uint8_t measureGraphicalValueWidth(GraphicalDisplayInterface* display) const {
        (void)display;
        return 0;
    }

    virtual bool hasGraphicalToggle() const { return false; }

    virtual bool graphicalToggleState() const { return false; }
};
