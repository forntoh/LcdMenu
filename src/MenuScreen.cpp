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

void MenuScreen::setCursor(MenuRenderer* renderer, uint8_t position) {
    uint8_t constrained = constrain(position, 0, itemCount - 1);
    if (constrained == cursor) {
        return;
    }
    uint8_t viewSize = renderer->maxRows;
    if (constrained < view) {
        view = constrained;
    } else if (constrained > (view + (viewSize - 1))) {
        view = constrained - (viewSize - 1);
    }
    cursor = position;
    draw(renderer);
}

void MenuScreen::draw(MenuRenderer* renderer) {
    renderer->restartTimer();
    for (uint8_t i = 0; i < renderer->maxRows; i++) {
        MenuItem* item = this->items[view + i];
        if (item == nullptr) {
            break;
        }
        syncIndicators(i, renderer);
        item->draw(renderer);
    }
}

void MenuScreen::syncIndicators(uint8_t index, MenuRenderer* renderer) {
    renderer->hasHiddenItemsAbove = index == 0 && view > 0;
    renderer->hasHiddenItemsBelow = index == renderer->maxRows - 1 && (view + renderer->maxRows) < itemCount;
    renderer->hasFocus = cursor == view + index;
    renderer->cursorRow = index;
}

bool MenuScreen::process(LcdMenu* menu, const unsigned char command) {
    MenuRenderer* renderer = menu->getRenderer();
    syncIndicators(cursor - view, renderer);
    if (items[cursor]->process(menu, command)) return true;
    switch (command) {
        case UP:
            renderer->viewShift = 0;
            up(renderer);
            return true;
        case DOWN:
            renderer->viewShift = 0;
            down(renderer);
            return true;
        case BACK:
            renderer->viewShift = 0;
            if (parent != NULL) {
                menu->setScreen(parent);
            }
            printLog(F("MenuScreen::back"));
            return true;
        case RIGHT:
            if (renderer->cursorCol >= renderer->getEffectiveCols()) {
                renderer->viewShift++;
                draw(renderer);
            }
            printLog(F("MenuScreen::right"), renderer->viewShift);
            return true;
        case LEFT:
            if (renderer->viewShift > 0) {
                renderer->viewShift--;
                draw(renderer);
            }
            printLog(F("MenuScreen::left"), renderer->viewShift);
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
    if (cursor < view) view--;

    draw(renderer);
    printLog(F("MenuScreen:up"), cursor);
}

void MenuScreen::down(MenuRenderer* renderer) {
    if (cursor == itemCount - 1) {
        printLog(F("MenuScreen:down"), cursor);
        return;
    }
    cursor++;
    if (cursor > view + renderer->maxRows - 1) view++;

    draw(renderer);
    printLog(F("MenuScreen:down"), cursor);
}

void MenuScreen::reset(MenuRenderer* renderer) {
    cursor = 0;
    view = 0;
    draw(renderer);
}

MenuScreen::MenuScreen(MenuItem** items) : items(items) {
    while (items[itemCount] != nullptr) {
        itemCount++;
    }
}