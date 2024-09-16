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
        config->menu->handle(UP);
    } else if (config->down && cmd == config->down) {
        config->menu->handle(DOWN);
    } else if (config->left && cmd == config->left) {
        config->menu->handle(LEFT);
    } else if (config->right && cmd == config->right) {
        config->menu->handle(RIGHT);
    } else if (config->enter && cmd == config->enter) {
        config->menu->handle(ENTER);
    } else if (config->back && cmd == config->back) {
        config->menu->handle(BACK);
#ifdef ItemInput_H
    } else if (config->clear && cmd == config->clear) {
        config->menu->handle(CLEAR);
    } else if (config->backspace && cmd == config->backspace) {
        config->menu->handle(BACKSPACE);
#endif
    } else {
        config->menu->handle(cmd);
        return false;
    }
    return true;
}
