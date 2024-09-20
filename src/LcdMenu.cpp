#include "LcdMenu.h"

DisplayInterface* LcdMenu::getDisplay() {
    return &display;
}

MenuScreen* LcdMenu::getScreen() {
    return screen;
}

void LcdMenu::setScreen(MenuScreen* screen) {
    this->screen = screen;
    display.clear();
    this->screen->draw(&display);
}

bool LcdMenu::process(const unsigned char c) {
    if (!enabled) {
        return false;
    }
    return screen->process(this, c);
};

void LcdMenu::reset() {
    this->screen->setCursor(&display, 0);
}

void LcdMenu::hide() {
    if (!enabled) {
        return;
    }
    enabled = false;
    display.clear();
}

void LcdMenu::show() {
    if (enabled) {
        return;
    }
    enabled = true;
    display.clear();
    screen->draw(&display);
}

uint8_t LcdMenu::getCursor() {
    return screen->getCursor();
}

void LcdMenu::setCursor(uint8_t cursor) {
    if (!enabled) {
        return;
    }
    screen->setCursor(&display, cursor);
}

MenuItem* LcdMenu::getItemAt(uint8_t position) {
    return screen->getItemAt(position);
}

void LcdMenu::refresh() {
    if (!enabled) {
        return;
    }
    screen->draw(&display);
}
