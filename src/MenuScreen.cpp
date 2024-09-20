#include "MenuScreen.h"

bool MenuScreen::back(MenuItem::Context context) {
    if (parent != NULL) {
        context.menu->setScreen(parent);
    }
    return true;
}
