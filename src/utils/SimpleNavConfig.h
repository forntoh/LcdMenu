#pragma once

#include <LcdMenu.h>

struct SimpleNavConfig {
    LcdMenu *menu;
    byte up;
    byte down;
    byte enter;
    byte back;
    byte left;
    byte right;
#ifdef ItemInput_H
    byte clear;
    byte backspace;
    const char *charset;
    uint8_t charsetSize;
    int8_t charsetPosition;
#endif
};

bool processWithSimpleCommand(SimpleNavConfig *config, byte cmd) {
    if (config->up && cmd == config->up) {
        if (config->menu->lcd.getEditModeEnabled()) {
#ifdef ItemInput_H
            if (config->charset) {
                config->charsetPosition =
                    (config->charsetPosition + 1) % config->charsetSize;
                config->menu->drawChar(
                    config->charset[config->charsetPosition]);
            } else if (!config->left)
#endif
                config->menu->left();
        } else {
            config->menu->up();
        }
    } else if (config->down && cmd == config->down) {
        if (config->menu->lcd.getEditModeEnabled()) {
#ifdef ItemInput_H
            if (config->charset) {
                config->charsetPosition = constrain(config->charsetPosition - 1,
                                                    0,
                                                    config->charsetSize - 1);
                config->menu->drawChar(
                    config->charset[config->charsetPosition]);
            } else if (!config->right)
#endif
                config->menu->right();
        }
        config->menu->down();
    } else if (config->left && cmd == config->left) {
        config->menu->left();
    } else if (config->right && cmd == config->right) {
        config->menu->right();
    } else if (config->enter && cmd == config->enter) {
#ifdef ItemInput_H
        if (config->charsetPosition != -1) {
            config->menu->type(config->charset[config->charsetPosition]);
            config->charsetPosition = -1;
        }
#endif
        config->menu->enter();
    } else if (config->back && cmd == config->back) {
        config->menu->back();
#ifdef ItemInput_H
    } else if (config->clear && cmd == config->clear) {
        config->menu->clear();
    } else if (config->backspace && cmd == config->backspace) {
        config->menu->backspace();
#endif
    } else {
        return false;
    }
    return true;
}
