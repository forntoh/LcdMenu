#pragma once

#include <stdint.h>

class GraphicalDisplayInterface;

/**
 * @brief Optional capabilities for items rendered on graphical displays.
 */
class GraphicalMenuItem {
  private:
    const uint8_t* itemFont = nullptr;

  public:
    static uint8_t capabilityId() { return 1; }

    virtual ~GraphicalMenuItem() {}

    virtual uint8_t measureGraphicalValueWidth(GraphicalDisplayInterface* display) const {
        (void)display;
        return 0;
    }

    virtual bool hasGraphicalToggle() const { return false; }

    virtual bool graphicalToggleState() const { return false; }

    virtual bool hasGraphicalListIndicator() const { return false; }

    virtual bool useTightGraphicalSelectionBox() const { return false; }

    virtual void setGraphicalFont(const uint8_t* font) { itemFont = font; }

    virtual const uint8_t* getGraphicalFont() const { return itemFont; }
};
