#pragma once

#include "LcdMenu.h"

class InputInterface {
  protected:
    LcdMenu *menu = NULL;

  public:
    InputInterface(LcdMenu *menu)
        : menu(menu) {
    }
    virtual void loop() = 0;
};
