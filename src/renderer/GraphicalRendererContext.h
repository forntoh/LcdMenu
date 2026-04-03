#pragma once

#include <stdint.h>

class MenuItem;
class GraphicalDisplayInterface;

/**
 * @brief Optional context API exposed by graphical renderers.
 *
 * This extension allows MenuScreen to share viewport and active-item context
 * with renderer implementations that need richer layout information.
 */
class GraphicalRendererContext {
  public:
    static uint8_t extensionId() { return 2; }

    virtual ~GraphicalRendererContext() {}

    virtual void setViewportContext(uint8_t viewStart, uint8_t totalItems) = 0;
    virtual void setValueAreaWidth(uint8_t width) = 0;
    virtual void setActiveItem(const MenuItem* item) = 0;
    virtual GraphicalDisplayInterface* getGraphicalDisplay() = 0;
};
