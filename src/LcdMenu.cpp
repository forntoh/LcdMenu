#include "LcdMenu.h"

MenuRenderer* LcdMenu::getRenderer() {
    return &renderer;
}

MenuScreen* LcdMenu::getScreen() {
    return screen;
}

void LcdMenu::setScreen(MenuScreen* screen) {
    printLog(F("LcdMenu::setScreen"));
    this->screen = screen;
    renderer.display->clear();
    this->screen->draw(&renderer);
}

bool LcdMenu::process(const unsigned char c) {
    if (!enabled) {
        return false;
    }
    return screen->process(this, c);
};

void LcdMenu::reset() {
    this->screen->setCursor(&renderer, 0);
}

void LcdMenu::hide() {
    if (!enabled) {
        return;
    }
    enabled = false;
    renderer.display->clear();
}

void LcdMenu::show() {
    if (enabled) {
        return;
    }
    enabled = true;
    renderer.display->clear();
    screen->draw(&renderer);
}

uint8_t LcdMenu::getCursor() {
    return screen->getCursor();
}

void LcdMenu::setCursor(uint8_t cursor) {
    if (!enabled) {
        return;
    }
    screen->setCursor(&renderer, cursor);
}

MenuItem* LcdMenu::getItemAt(uint8_t position) {
    return screen->getItemAt(position);
}

void LcdMenu::refresh() {
    if (!enabled) {
        return;
    }
    screen->draw(&renderer);
}
