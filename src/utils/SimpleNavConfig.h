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
        config->menu->process(UP);
    } else if (config->down && cmd == config->down) {
        config->menu->process(DOWN);
    } else if (config->left && cmd == config->left) {
        config->menu->process(LEFT);
    } else if (config->right && cmd == config->right) {
        config->menu->process(RIGHT);
    } else if (config->enter && cmd == config->enter) {
        config->menu->process(ENTER);
    } else if (config->back && cmd == config->back) {
        config->menu->process(BACK);
#ifdef ItemInput_H
    } else if (config->clear && cmd == config->clear) {
        config->menu->process(CLEAR);
    } else if (config->backspace && cmd == config->backspace) {
        config->menu->process(BACKSPACE);
#endif
    } else {
        config->menu->process(cmd);
        return false;
    }
    return true;
}
