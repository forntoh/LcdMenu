#pragma once

#include "input/InputInterface.h"
#include <LcdMenu.h>
#include <SimpleRotary.h>

// Duration (ms) to consider a long press.
#define LONG_PRESS_DURATION 1000
// Duration (ms) to consider a double press.
#define DOUBLE_PRESS_THRESHOLD 500

class SimpleRotaryAdapter : public InputInterface {
  protected:
    /**
     * @brief Pointer to the rotary encoder instance.
     */
    SimpleRotary* encoder;
    /**
     * @brief The last time the button was pressed.
     */
    unsigned long lastPressTime;
    /**
     * @brief Flag to indicate if an enter action is pending.
     */
    bool pendingEnter;

  public:
    SimpleRotaryAdapter(LcdMenu* menu, SimpleRotary* encoder) : InputInterface(menu), encoder(encoder) {}
    /**
     * @brief Handles rotary encoder navigation in the LCD menu.
     */
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
                if (currentTime - lastPressTime < DOUBLE_PRESS_THRESHOLD) {
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

        // Check if the DOUBLE_PRESS_THRESHOLD has elapsed for pending enter action
        if ((!menu->lcd.getEditModeEnabled() && pendingEnter) ||
            (pendingEnter && (currentTime - lastPressTime >= DOUBLE_PRESS_THRESHOLD))) {
            menu->process(ENTER);  // Call ENTER action (short press)
            pendingEnter = false;
        }
    }
};
