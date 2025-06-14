#pragma once

#include "InputInterface.h"
#include "RepeatState.h"
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
 * @param repeatDelay Delay before the first repeat command is sent (default is 0).
 * @param repeatInterval Interval between repeated commands (default is 0).
 */
class ButtonAdapter : public InputInterface {
  private:
    Button* button;
    byte command;
    RepeatState repeat;
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
          repeat(repeatDelay, repeatInterval) {}

    void observe() override {
        if (button->pressed()) {
            menu->process(command);
            holding = true;
            if (repeat.enabled()) {
                repeat.start(millis());
            }
        } else if (button->released()) {
            holding = false;
            repeat.reset();
        } else if (repeat.enabled() && holding && repeat.shouldRepeat(millis())) {
            menu->process(command);
        }
    }
};
