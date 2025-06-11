#pragma once
//
// Rotary encoder configuration
//
/**
 * @brief Duration for a long press in milliseconds.
 *
 * This value defines the duration (in milliseconds) that a button must be pressed
 * to be considered a long press. The default value is 1000 milliseconds.
 */
#ifndef LONG_PRESS_DURATION
#define LONG_PRESS_DURATION 1000
#endif
/**
 * @brief Threshold for detecting a double press in milliseconds.
 *
 * This value defines the maximum time interval (in milliseconds) between two
 * consecutive button presses to be considered a double press. The default value
 * is 300 milliseconds.
 */
#ifndef DOUBLE_PRESS_THRESHOLD
#define DOUBLE_PRESS_THRESHOLD 300
#endif
//
#include "InputInterface.h"
#include <SimpleRotary.h>

/**
 * @class SimpleRotaryAdapter
 * @brief Adapter for handling rotary input for an LCD menu.
 *
 * This class interfaces with a rotary encoder to manage user input
 * for navigating and selecting options in an LCD menu.
 *
 * It processes rotary encoder rotations to navigate through menu options
 * and handles button presses for various actions, including:
 * - Short press for selecting an option
 * - Long press for going back
 * - Double press for backspacing
 *
 * The values for long press duration (defined as #LONG_PRESS_DURATION) and
 * double press threshold (defined as #DOUBLE_PRESS_THRESHOLD) can be
 * overwritten by defining new ones with #define.
 *
 * @param menu Pointer to the LcdMenu instance that this adapter will control.
 * @param encoder Pointer to the SimpleRotary instance representing the rotary encoder.
 */
class SimpleRotaryAdapter : public InputInterface {
  private:
    unsigned long lastPressTime = 0;  // Last time the button was pressed
    bool pendingEnter = false;        // Flag to indicate if an enter action is pending
    SimpleRotary* encoder;            // Pointer to the SimpleRotary instance

  public:
    SimpleRotaryAdapter(LcdMenu* menu, SimpleRotary* encoder)
        : InputInterface(menu), encoder(encoder) {
    }

    void observe() override {
        // Handle rotary encoder rotation
        uint8_t rotation = encoder->rotate();
        if (rotation == 1) {
            menu->process(DOWN);  // Call DOWN action
        } else if (rotation == 2) {
            menu->process(UP);  // Call UP action
        }

        // Handle button press (short, long, and double press)
        uint8_t pressType = encoder->pushType(LONG_PRESS_DURATION);
        unsigned long currentTime = millis();

        if (pressType == 1) {
            if (pendingEnter) {
                if (DOUBLE_PRESS_THRESHOLD > 0 && currentTime - lastPressTime < DOUBLE_PRESS_THRESHOLD) {
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
        if ((!menu->getRenderer()->isInEditMode() && pendingEnter) || (pendingEnter && (currentTime - lastPressTime >= DOUBLE_PRESS_THRESHOLD))) {
            menu->process(ENTER);  // Call ENTER action (short press)
            pendingEnter = false;
        }
    }
};
