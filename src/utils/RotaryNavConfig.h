#pragma once

#include <LcdMenu.h>
#include <SimpleRotary.h>

/**
 * @brief Configuration for rotary encoder navigation in the LCD menu.
 *
 * @param encoder Pointer to the rotary encoder instance
 * @param menu Pointer to the LCD menu instance
 * @param doublePressThreshold Duration (ms) to consider a double press
 * @param longPressDuration Duration (ms) to consider a long press
 * @param lastPressTime The last time the button was pressed
 * @param pendingEnter Flag to indicate if an enter action is pending
 */
struct RotaryNavConfig {
    SimpleRotary *encoder;
    LcdMenu *menu;
    uint16_t longPressDuration;
    uint16_t doublePressThreshold;
    unsigned long lastPressTime;
    bool pendingEnter;
};

/**
 * @brief Handles rotary encoder navigation in the LCD menu.
 *
 * @param config Pointer to the RotaryNavConfig struct
 */
void processWithRotaryEncoder(RotaryNavConfig *config) {
    // Handle rotary encoder rotation
    uint8_t rotation = config->encoder->rotate();
    if (rotation == 1) {
        config->menu->handle(DOWN); // Call DOWN action
    } else if (rotation == 2) {
        config->menu->handle(UP); // Call UP action
    }

    // Handle button press (short, long, and double press)
    uint8_t pressType = config->encoder->pushType(config->longPressDuration);
    unsigned long currentTime = millis();

    if (pressType == 1) {
        if (config->pendingEnter) {
            if (config->doublePressThreshold > 0 &&
                currentTime - config->lastPressTime < config->doublePressThreshold) {
                config->menu->handle(BACKSPACE); // Call BACKSPACE action (double press)
                config->pendingEnter = false;
            }
        } else {
            config->pendingEnter = true;
            config->lastPressTime = currentTime;
        }
    } else if (pressType == 2) {
        config->menu->handle(BACK); // Call BACK action (long press)
        config->pendingEnter = false;
    }

    // Check if the doublePressThreshold has elapsed for pending enter action
    if ((!config->menu->lcd.getEditModeEnabled() && config->pendingEnter) ||
        (config->pendingEnter && (currentTime - config->lastPressTime >= config->doublePressThreshold))) {
        config->menu->handle(ENTER); // Call ENTER action (short press)
        config->pendingEnter = false;
    }
}