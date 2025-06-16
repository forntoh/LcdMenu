#include "MenuScreen.h"
#include "display/GraphicalDisplayInterface.h"
#include "renderer/GraphicalDisplayRenderer.h"

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
    if (items.empty()) {
        cursor = 0;
        draw(renderer);
        return;
    }
    uint8_t constrained = constrain(position, 0, items.size() - 1);
    if (constrained == cursor) {
        return;
    }
    uint8_t viewSize = renderer->getMaxRows();
    if (constrained < view) {
        view = constrained;
    } else if (constrained > (view + (viewSize - 1))) {
        view = constrained - (viewSize - 1);
    }
    cursor = constrained;
    draw(renderer);
}

void MenuScreen::draw(MenuRenderer* renderer) {
    GraphicalDisplayInterface* gDisplay =
        renderer->display->isGraphical()
            ? static_cast<GraphicalDisplayInterface*>(renderer->display)
            : nullptr;
    GraphicalDisplayRenderer* gRenderer =
        gDisplay ? static_cast<GraphicalDisplayRenderer*>(renderer) : nullptr;
    if (gRenderer) {
        gDisplay->clearBuffer();
        uint8_t widest = 0;
        for (uint8_t i = 0; i < renderer->getMaxRows() && (view + i) < items.size(); i++) {
            MenuItem* it = items[view + i];
            uint8_t w = it->measureValueWidth(gDisplay);
            if (w > widest) widest = w;
        }
        uint8_t maxAllowed = gDisplay->getDisplayWidth() / 2;
        if (widest > maxAllowed) widest = maxAllowed;
        gRenderer->setValueWidth(widest);
    }
    for (uint8_t i = 0; i < renderer->getMaxRows() && (view + i) < items.size(); i++) {
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
    renderer->hasHiddenItemsBelow = index == renderer->getMaxRows() - 1 && (view + renderer->getMaxRows()) < items.size();
    renderer->hasFocus = cursor == view + index;
    renderer->cursorRow = index;
    renderer->viewStart = view;
    renderer->totalItems = items.size();
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
            LOG(F("MenuScreen::back"));
            return true;
        case RIGHT:
            if (renderer->cursorCol >= renderer->getMaxCols() - 1) {
                renderer->viewShift++;
                draw(renderer);
            }
            LOG(F("MenuScreen::right"), renderer->viewShift);
            return true;
        case LEFT:
            if (renderer->viewShift > 0) {
                renderer->viewShift--;
                draw(renderer);
            }
            LOG(F("MenuScreen::left"), renderer->viewShift);
            return true;
        default:
            return false;
    }
}

void MenuScreen::up(MenuRenderer* renderer) {
    if (cursor > 0) {
        if (--cursor < view) view--;
        draw(renderer);
    }
    LOG(F("MenuScreen::up"), cursor);
}

void MenuScreen::down(MenuRenderer* renderer) {
    if (items.empty()) {
        cursor = 0;
        draw(renderer);
        return;
    }
    if (cursor < items.size() - 1) {
        if (++cursor > view + renderer->getMaxRows() - 1) view++;
        draw(renderer);
    }
    LOG(F("MenuScreen::down"), cursor);
}

void MenuScreen::reset(MenuRenderer* renderer) {
    cursor = 0;
    view = 0;
    draw(renderer);
}

MenuScreen::MenuScreen(const std::vector<MenuItem*>& items) : items(items) {}

void MenuScreen::addItem(MenuItem* item) {
    items.push_back(item);
}

void MenuScreen::addItemAt(uint8_t position, MenuItem* item) {
    if (position <= items.size()) {
        items.insert(items.begin() + position, item);
    }
}

void MenuScreen::removeItemAt(uint8_t position) {
    if (position < items.size()) {
        items.erase(items.begin() + position);
    }
}

void MenuScreen::removeLastItem() {
    if (items.size() > 0) {
        items.pop_back();
    }
}

void MenuScreen::clear() {
    items.clear();
}

void MenuScreen::poll(MenuRenderer* renderer, uint16_t pollInterval) {
    static unsigned long lastPollTime = 0;
    if (millis() - lastPollTime >= pollInterval) {
        for (uint8_t i = 0; i < renderer->getMaxRows() && (view + i) < items.size(); i++) {
            MenuItem* item = this->items[view + i];
            if (item == nullptr || !item->polling || renderer->isInEditMode()) continue;
            syncIndicators(i, renderer);
            item->draw(renderer);
        }
        lastPollTime = millis();
    }
}