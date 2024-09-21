#include "MenuScreen.h"

bool MenuScreen::process(LcdMenu* menu, const unsigned char command) {
    DisplayInterface* display = menu->getDisplay();
    if (items[cursor]->process(menu, command)) {
        return true;
    }
    switch (command) {
        case UP:
            up(display);
            return true;
        case DOWN:
            down(display);
            return true;
        case BACK:
            if (parent != NULL) {
                menu->setScreen(parent);
            }
            printLog(F("MenuScreen::back"));
            return true;
        default:
            return false;
    }
}
