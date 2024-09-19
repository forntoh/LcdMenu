#include "MenuScreen.h"

bool MenuScreen::back(MenuItem::Context context) {
    if (parent != NULL) {
        context.menu->setCurrentScreen(parent);
    }
    return true;
}
