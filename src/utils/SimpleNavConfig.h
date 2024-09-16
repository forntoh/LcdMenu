#pragma once

#include <LcdMenu.h>

struct SimpleNavConfig {
    LcdMenu* menu;
    byte up;
    byte down;
    byte enter;
    byte back;
    byte left;
    byte right;
#ifdef ItemInput_H
    byte clear;
    byte backspace;
#endif
};

bool processWithSimpleCommand(SimpleNavConfig* config, byte cmd) {
    if (config->up && cmd == config->up) {
        config->menu->up();
    } else if (config->down && cmd == config->down) {
        config->menu->down();
    } else if (config->left && cmd == config->left) {
        config->menu->left();
    } else if (config->right && cmd == config->right) {
        config->menu->right();
    } else if (config->enter && cmd == config->enter) {
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
