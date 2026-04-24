#pragma once

#include <stdint.h>

/**
 * @brief Optional renderer interface for highlighting a value substring.
 */
class GraphicalValueSelectionRenderer {
  public:
    static uint8_t extensionId() { return 4; }

    virtual ~GraphicalValueSelectionRenderer() {}

    virtual void setValueSelection(uint8_t start, uint8_t length) = 0;
    virtual void clearValueSelection() = 0;
};
