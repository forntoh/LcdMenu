#include "LcdMenu.h"

void LcdMenu::setCurrentScreen(MenuScreen* screen) {
    currentScreen = screen;
    display.clear();
    currentScreen->draw(&display);
}

void LcdMenu::initialize(MenuScreen* screen) {
    display.begin();
    currentScreen = screen;
    currentScreen->draw(&display);
}

bool LcdMenu::process(const unsigned char c) {
    MenuItem::Context context{this, &display, c};
    return currentScreen->process(context);
};

void LcdMenu::resetMenu() {
    this->currentScreen->reset(&display);
}

void LcdMenu::hide() {
    enableUpdate = false;
    display.clear();
}

void LcdMenu::show() {
    enableUpdate = true;
    display.clear();
    currentScreen->draw(&display);
}

uint8_t LcdMenu::getCursorPosition() {
    return this->currentScreen->getCursor();
}

void LcdMenu::setCursorPosition(uint8_t position) {
    this->currentScreen->setCursor(&display, position);
}

MenuItem* LcdMenu::getItemAt(uint8_t position) {
    return currentScreen->getItemAt(position);
}

void LcdMenu::refresh() {
    if (!enableUpdate) {
        return;
    }
    currentScreen->draw(&display);
    display.drawCursor();
}
