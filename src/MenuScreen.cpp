#include "MenuScreen.h"

#include "LcdMenu.h"

MenuScreen::MenuScreen(uint8_t itemsCount, MenuItem* items[])
    : items(items), itemsCount(itemsCount) {}

void MenuScreen::initialize(LcdMenu* menu, DisplayInterface* display) {
    this->menu = menu;
    this->display = display;
    viewSize = display->getMaxRows();
    for (uint8_t i = 0; i < itemsCount; i++) {
        items[i]->initialize(menu, this, display);
    }
}

void MenuScreen::setParent(MenuScreen* newParent) { parent = newParent; }
uint8_t MenuScreen::getCursor() { return cursor; }

void MenuScreen::setCursor(uint8_t position) {
    uint8_t constrained = constrain(position, 0, itemsCount - 1);
    if (constrained == cursor) {
        return;
    }
    if (constrained < view) {
        view = constrained;
    } else if (constrained > (view + (viewSize - 1))) {
        view = constrained - (viewSize - 1);
    }
    cursor = position;
    draw();
}

MenuItem* MenuScreen::getItemAt(uint8_t position) { return items[position]; }

bool MenuScreen::up() {
    if (items[cursor]->up()) {
        return true;
    }
    if (cursor == 0) {
        return false;
    }
    cursor--;
    if (cursor < view) {
        view--;
        draw();
    } else {
        display->moveCursor(cursor - view);
    }
    return true;
}

bool MenuScreen::down() {
    if (items[cursor]->down()) {
        return true;
    }
    if (cursor == itemsCount - 1) {
        return false;
    }
    cursor++;
    if (cursor > view + viewSize - 1) {
        view++;
        draw();
    } else {
        display->moveCursor(cursor - view);
    }
    return true;
}

bool MenuScreen::enter() {
    return items[cursor]->enter();
}

bool MenuScreen::back() {
    if (items[cursor]->back()) {
        return true;
    }
    if (parent != NULL) {
        menu->setCurrentScreen(parent);
    }
    return true;
}

bool MenuScreen::left() { return items[cursor]->left(); }
bool MenuScreen::right() { return items[cursor]->right(); }
bool MenuScreen::backspace() { return items[cursor]->backspace(); }
bool MenuScreen::type(const char c) { return items[cursor]->type(c); }
bool MenuScreen::clear() { return items[cursor]->clear(); }

void MenuScreen::draw() {
    for (uint8_t i = 0; i < min(viewSize, itemsCount - view); i++) {
        this->items[view + i]->draw(i);
    }
    if (view == 0) {
        display->clearUpIndicator();
    } else {
        display->drawUpIndicator();
    }
    if (view + viewSize < itemsCount - 1) {
        display->drawDownIndicator();
    } else {
        display->clearDownIndicator();
    }
    display->moveCursor(cursor - view);
    display->drawCursor();  // In case if currentPosition was not changed
                            // between screens
}

void MenuScreen::reset() {
    cursor = 0;
    view = 0;
    draw();
}
