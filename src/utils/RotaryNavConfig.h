#pragma once

#include <LcdMenu.h>
#include <SimpleRotary.h>

/**
 * @brief Configuration for rotary encoder navigation in the LCD menu.
 *
 * @param encoder Pointer to the rotary encoder instance
 * @param menu Pointer to the LCD menu instance
 * @param longPressDuration Duration (ms) to consider a long press
 */
struct RotaryNavConfig {
    SimpleRotary *encoder;
    LcdMenu *menu;
    uint16_t longPressDuration;
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
        config->menu->down(); // Call DOWN action
    } else if (rotation == 2) {
        config->menu->up(); // Call UP action
    }
    // Handle button press (short and long press)
    uint8_t pressType = config->encoder->pushType(config->longPressDuration);
    if (pressType == 1) {
        config->menu->enter(); // Call ENTER action (short press)
    } else if (pressType == 2) {
        config->menu->back(); // Call BACK action (long press)
    }
}
