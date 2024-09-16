#pragma once

#include "LcdMenu.h"

/**
 * @brief Input device for menu.
 * Main goal is to hide implementation of input device(s) and convert from their events
 * into 1 byte command to `LcdMenu`.
 */
class InputInterface {
  protected:
    LcdMenu *menu = NULL;

  public:
    InputInterface(LcdMenu *menu)
        : menu(menu) {
    }
    virtual void loop() = 0;
};
