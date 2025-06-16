#include "LcdMenu.h"
#include "display/GraphicalDisplayInterface.h"

MenuRenderer* LcdMenu::getRenderer() {
    return &renderer;
}

MenuScreen* LcdMenu::getScreen() {
    return screen;
}

void LcdMenu::setScreen(MenuScreen* screen) {
    LOG(F("LcdMenu::setScreen"));
    this->screen = screen;
    renderer.display->clear();
    this->screen->reset(&renderer);
    this->screen->draw(&renderer);
    if (renderer.display->isGraphical())
        static_cast<GraphicalDisplayInterface*>(renderer.display)->sendBuffer();
}

bool LcdMenu::process(const unsigned char c) {
    if (!enabled) {
        return false;
    }
    renderer.restartTimer();
    bool handled = screen->process(this, c);
    if (handled && renderer.display->isGraphical())
        static_cast<GraphicalDisplayInterface*>(renderer.display)->sendBuffer();
    return handled;
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
    if (renderer.display->isGraphical())
        static_cast<GraphicalDisplayInterface*>(renderer.display)->sendBuffer();
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
    if (renderer.display->isGraphical())
        static_cast<GraphicalDisplayInterface*>(renderer.display)->sendBuffer();
}

void LcdMenu::poll(uint16_t pollInterval) {
    if (!enabled || pollInterval == 0) {
        return;
    }
    screen->poll(&renderer, pollInterval < 100 ? 100 : pollInterval);
}
bool LcdMenu::isEnabled() const {
    return enabled;
}