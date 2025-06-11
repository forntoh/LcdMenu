#pragma once

#include "InputInterface.h"
#include <Button.h>

/**
 * @class ButtonAdapter
 * @brief Adapter class to handle button input for an LCD menu.
 *
 * This class implements the InputInterface to provide button input handling
 * for an LCD menu. It observes button presses and processes commands accordingly.
 *
 * @details
 * The ButtonAdapter class encapsulates a Button object and a command byte.
 * It initializes the button and observes its state to trigger menu commands when
 * the button is pressed.
 *
 * @param menu Pointer to the LcdMenu object that this adapter will interact with.
 * @param button The Button object that will be used for input.
 * @param command The command byte that will be sent to the menu when the button is pressed.
 */
class ButtonAdapter : public InputInterface {
  private:
    Button* button;
    byte command;
    unsigned long repeatDelay;
    unsigned long repeatInterval;
    unsigned long pressStart = 0;
    unsigned long lastRepeat = 0;
    bool holding = false;

  public:
    ButtonAdapter(
        LcdMenu* menu,
        Button* button,
        byte command,
        unsigned long repeatDelay = 0,
        unsigned long repeatInterval = 0)
        : InputInterface(menu),
          button(button),
          command(command),
          repeatDelay(repeatDelay),
          repeatInterval(repeatInterval) {}

    void observe() override {
        if (button->pressed()) {
            menu->process(command);
            holding = true;
            if (repeatDelay && repeatInterval) {
                pressStart = millis();
                lastRepeat = 0;
            }
        } else if (button->released()) {
            holding = false;
            pressStart = 0;
            lastRepeat = 0;
        } else if (repeatDelay && repeatInterval && holding) {
            unsigned long now = millis();
            if (now - pressStart >= repeatDelay &&
                (lastRepeat == 0 || now - lastRepeat >= repeatInterval)) {
                menu->process(command);
                lastRepeat = now;
            }
        }
    }
};
