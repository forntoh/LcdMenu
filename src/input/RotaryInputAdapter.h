#pragma once

#include "InputInterface.h"
#include <SimpleRotary.h>

class RotaryInputAdapter : public InputInterface {
  private:
    uint16_t longPressDuration;       // Duration to consider a long press
    uint16_t doublePressThreshold;    // Duration to consider a double press
    unsigned long lastPressTime = 0;  // Last time the button was pressed
    bool pendingEnter = false;        // Flag to indicate if an enter action is pending

  public:
    SimpleRotary* encoder;

    /**
     * @brief Adapter for handling rotary input for an LCD menu.
     *
     * This class interfaces with a rotary encoder to manage user input
     * for navigating and selecting options in an LCD menu.
     *
     * @param menu Pointer to the LcdMenu instance that this adapter will control.
     * @param encoder Pointer to the SimpleRotary instance representing the rotary encoder.
     * @param longPressDuration Duration in milliseconds to recognize a long press (default is 1000 ms).
     * @param doublePressThreshold Duration in milliseconds to recognize a double press (default is 300 ms).
     */
    RotaryInputAdapter(
        LcdMenu* menu,
        SimpleRotary* encoder,
        uint16_t longPressDuration = 1000,
        uint16_t doublePressThreshold = 300)

        : InputInterface(menu),
          encoder(encoder),
          longPressDuration(longPressDuration),
          doublePressThreshold(doublePressThreshold) {};

    void observe() override {
        // Handle rotary encoder rotation
        uint8_t rotation = encoder->rotate();
        if (rotation == 1) {
            menu->process(DOWN);  // Call DOWN action
        } else if (rotation == 2) {
            menu->process(UP);  // Call UP action
        }

        // Handle button press (short, long, and double press)
        uint8_t pressType = encoder->pushType(longPressDuration);
        unsigned long currentTime = millis();

        if (pressType == 1) {
            if (pendingEnter) {
                if (doublePressThreshold > 0 &&
                    currentTime - lastPressTime < doublePressThreshold) {
                    menu->process(BACKSPACE);  // Call BACKSPACE action (double press)
                    pendingEnter = false;
                }
            } else {
                pendingEnter = true;
                lastPressTime = currentTime;
            }
        } else if (pressType == 2) {
            menu->process(BACK);  // Call BACK action (long press)
            pendingEnter = false;
        }

        // Check if the doublePressThreshold has elapsed for pending enter action
        if ((!menu->lcd.getEditModeEnabled() && pendingEnter) ||
            (pendingEnter && (currentTime - lastPressTime >= doublePressThreshold))) {
            menu->process(ENTER);  // Call ENTER action (short press)
            pendingEnter = false;
        }
    }
};