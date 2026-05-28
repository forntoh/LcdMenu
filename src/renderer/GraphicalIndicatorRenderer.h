#pragma once

#include <stdint.h>

/**
 * @brief Optional renderer interface for graphical row indicators.
 */
class GraphicalIndicatorRenderer {
  public:
    static uint8_t extensionId() { return 3; }

    virtual ~GraphicalIndicatorRenderer() {}

    virtual void drawSubMenuIndicator() = 0;
    virtual void drawListIndicator() = 0;
};
