#pragma once

#include "LcdMenu.h"

/**
 * @class InputInterface
 * @brief Abstract base class for input handling in the LcdMenu system.
 *
 * This class provides an interface for observing input events and interacting
 * with the LcdMenu. Derived classes must implement the observe() method to
 * handle specific input mechanisms.
 *
 * @note This class is intended to be subclassed and cannot be instantiated directly.
 *
 * @param menu Pointer to an LcdMenu object that this input interface will interact with.
 */
class InputInterface {
  protected:
    LcdMenu* menu = NULL;

  public:
    InputInterface(LcdMenu* menu) : menu(menu) {}

    virtual ~InputInterface() = default;

    virtual void observe() = 0;
};
