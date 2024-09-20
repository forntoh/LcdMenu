#include "MenuScreen.h"

bool MenuScreen::back(MenuItem::Context context) {
    if (parent != NULL) {
        context.menu->setScreen(parent);
    }
    printLog(F("MenuScreen::back"));
    return true;
}
