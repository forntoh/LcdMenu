#include "MenuScreen.h"

void MenuScreen::setParent(MenuScreen* parent) {
    this->parent = parent;
}

uint8_t MenuScreen::getCursor() {
    return cursor;
}

MenuItem* MenuScreen::getItemAt(uint8_t position) {
    return items[position];
}

MenuItem* MenuScreen::operator[](const uint8_t position) {
    return getItemAt(position);
}

uint8_t MenuScreen::itemsCount() {
    uint8_t i = 0;
    while (items[i] != nullptr) {
        i++;
    }
    return i;
}

void MenuScreen::setCursor(DisplayInterface* display, uint8_t position) {
    uint8_t constrained = constrain(position, 0, itemsCount() - 1);
    if (constrained == cursor) {
        return;
    }
    uint8_t viewSize = display->getMaxRows();
    if (constrained < view) {
        view = constrained;
    } else if (constrained > (view + (viewSize - 1))) {
        view = constrained - (viewSize - 1);
    }
    cursor = position;
    draw(display);
}

void MenuScreen::draw(DisplayInterface* display) {
    bool notFullView = false;
    for (uint8_t i = 0; i < display->getMaxRows(); i++) {
        MenuItem* item = this->items[view + i];
        if (item == nullptr) {
            notFullView = true;
            break;
        }
        item->draw(display, i);
    }
    if (view == 0) {
        display->clearUpIndicator();
    } else {
        display->drawUpIndicator();
    }
    if (notFullView || items[view + display->getMaxRows()] == nullptr) {
        display->clearDownIndicator();
    } else {
        display->drawDownIndicator();
    }
    display->moveCursor(cursor - view);
    display->drawCursor();  // In case if currentPosition was not changed between screens
}

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

void MenuScreen::up(DisplayInterface* display) {
    if (cursor == 0) {
        printLog(F("MenuScreen:up"), cursor);
        return;
    }
    cursor--;
    if (cursor < view) {
        view--;
        draw(display);
    } else {
        display->moveCursor(cursor - view);
    }
    printLog(F("MenuScreen:up"), cursor);
}

void MenuScreen::down(DisplayInterface* display) {
    if (cursor == itemsCount() - 1) {
        printLog(F("MenuScreen:down"), cursor);
        return;
    }
    cursor++;
    if (cursor > view + display->getMaxRows() - 1) {
        view++;
        draw(display);
    } else {
        display->moveCursor(cursor - view);
    }
    printLog(F("MenuScreen:down"), cursor);
}

void MenuScreen::reset(DisplayInterface* display) {
    cursor = 0;
    view = 0;
    draw(display);
}