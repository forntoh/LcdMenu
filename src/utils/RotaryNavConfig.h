#pragma once

#include <LcdMenu.h>
#include <SimpleRotary.h>

/**
 * @brief Configuration for rotary encoder navigation in the LCD menu.
 *
 * @param encoder Pointer to the rotary encoder instance
 * @param menu Pointer to the LCD menu instance
 * @param longPressDuration Duration (ms) to consider a long press
 * @param charset Pointer to the character set
 * @param charsetSize Size of the character set
 * @param charsetPosition Position in the character set
 */
struct RotaryNavConfig {
    SimpleRotary *encoder;
    LcdMenu *menu;
    uint16_t longPressDuration;
#ifdef ItemInput_H
    const char *charset;
    uint8_t charsetSize;
    int8_t charsetPosition;
#endif
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
        if (config->menu->lcd.getEditModeEnabled()) {
#ifdef ItemInput_H
            if (config->charset) {
                config->charsetPosition =
                    (config->charsetPosition + 1) % config->charsetSize;
                config->menu->drawChar(
                    config->charset[config->charsetPosition]);
            } else
#endif
                config->menu->right(); // Call RIGHT action in edit mode
        } else if (!config->menu->lcd.getEditModeEnabled()) {
            config->menu->down(); // Call DOWN action in normal mode
        }
    } else if (rotation == 2) {
        if (config->menu->lcd.getEditModeEnabled()) {
#ifdef ItemInput_H
            if (config->charset) {
                config->charsetPosition = constrain(config->charsetPosition - 1, 0, config->charsetSize - 1);
                config->menu->drawChar(
                    config->charset[config->charsetPosition]);
            } else
#endif
                config->menu->left(); // Call LEFT action in edit mode
        } else if (!config->menu->lcd.getEditModeEnabled()) {
            config->menu->up(); // Call UP action in normal mode
        }
    }

    // Handle button press (short and long press)
    uint8_t pressType = config->encoder->pushType(config->longPressDuration);
    if (pressType == 1) {
#ifdef ItemInput_H
        if (config->charsetPosition != -1) {
            config->menu->type(config->charset[config->charsetPosition]);
            config->charsetPosition = -1;
        }
#endif
        config->menu->enter(); // Call ENTER action (short press)
    } else if (pressType == 2) {
        config->menu->back(); // Call BACK action (long press)
    }
}
