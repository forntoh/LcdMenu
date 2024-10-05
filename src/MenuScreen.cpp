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

void MenuScreen::setCursor(MenuRenderer* renderer, uint8_t position) {
    uint8_t constrained = constrain(position, 0, itemsCount() - 1);
    if (constrained == cursor) {
        return;
    }
    uint8_t viewSize = renderer->getMaxRows();
    if (constrained < view) {
        view = constrained;
    } else if (constrained > (view + (viewSize - 1))) {
        view = constrained - (viewSize - 1);
    }
    cursor = position;
    draw(renderer);
}

void MenuScreen::draw(MenuRenderer* renderer) {
    renderer->setItemCount(itemCount);
    renderCursor(renderer);

    for (uint8_t i = 0; i < renderer->getMaxRows(); i++) {
        MenuItem* item = this->items[view + i];
        if (item == nullptr) {
            break;
        }
        item->draw(renderer, view + i, i);
    }

    renderCursor(renderer);
}

bool MenuScreen::process(LcdMenu* menu, const unsigned char command) {
    MenuRenderer* renderer = menu->getRenderer();
    if (items[cursor]->process(menu, command)) {
        return true;
    }
    switch (command) {
        case UP:
            up(renderer);
            return true;
        case DOWN:
            down(renderer);
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

void MenuScreen::up(MenuRenderer* renderer) {
    if (cursor == 0) {
        printLog(F("MenuScreen:up"), cursor);
        return;
    }
    cursor--;
    if (cursor < view) {
        view--;
        draw(renderer);
    } else {
        renderCursor(renderer);
    }
    printLog(F("MenuScreen:up"), cursor);
}

void MenuScreen::down(MenuRenderer* renderer) {
    if (cursor == itemsCount() - 1) {
        printLog(F("MenuScreen:down"), cursor);
        return;
    }
    cursor++;
    if (cursor > view + renderer->getMaxRows() - 1) {
        view++;
        draw(renderer);
    } else {
        renderCursor(renderer);
    }
    printLog(F("MenuScreen:down"), cursor);
}

void MenuScreen::renderCursor(MenuRenderer* renderer) {
    if (renderer->getCursorRow() != cursor - view) {
        renderer->clearCursor();
        renderer->moveCursor(0, cursor - view);
        renderer->drawCursor();
    }
}

void MenuScreen::reset(MenuRenderer* renderer) {
    cursor = 0;
    view = 0;
    draw(renderer);
}