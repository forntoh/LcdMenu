#pragma once

#include "LcdMenu.h"
#include "display/DisplayInterface.h"
#include "utils/constants.h"
#include <MenuItem.h>
#include <utils/utils.h>

/**
 * @class MenuScreen
 * @brief Represents single screen with number of menu items.
 * Contains logic of navigating between items. Stores current cursor and view.
 */
class MenuScreen {
    friend LcdMenu;

  private:
    /**
     * @brief Previous screen.
     * When `BACK` command received this screen will be shown.
     */
    MenuScreen* parent = NULL;
    /**
     * @brief The menu items to be displayed on screen.
     * These items will be drawn on display.
     */
    MenuItem** items = NULL;
    /**
     * @brief Cursor position.
     *
     *     X X
     *     X X X X X
     * ┌───────────────┐
     * │   X X X       │
     * │   X X X X     │
     * │ > X X X X X X │ <── cursor
     * │   X X X X X   │
     * └───────────────┘
     *     X X X X
     *     X X X X X X
     *
     * When `up` or `down` then this position will be moved over the items accordingly.
     * Always in range [`view`, `view` + `display.getMaxRows()` - 1].
     */
    uint8_t cursor = 0;
    /**
     * @brief First visible item's position in the menu array.
     *
     *     X X
     *     X X X X X
     * ┌───────────────┐
     * │   X X X       │ <── view
     * │   X X X X     │
     * │ > X X X X X X │
     * │   X X X X X   │
     * └───────────────┘
     *     X X X X
     *     X X X X X X
     *
     * Is moved when `cursor` crosses the borders of visible area.
     * When number of items < `display.getMaxRows()` this index should be 0.
     * The size of the view is always the same and equals to `display.getMaxRows()`.
     */
    uint8_t view = 0;

  public:
    /**
     * Constructor
     */
    MenuScreen(MenuItem** items) : items(items) {}
    /**
     * @brief Set new paren screen.
     */
    void setParent(MenuScreen* parent) {
        this->parent = parent;
    }
    /**
     * @brief Get current cursor position.
     */
    uint8_t getCursor() {
        return cursor;
    }
    /**
     * @brief Get a `MenuItem` at position.
     * @return `MenuItem` - item at `position`
     */
    MenuItem* getItemAt(uint8_t position) {
        return items[position];
    }
    /**
     * @brief Get a `MenuItem` at position using operator function
     * e.g `menu[menu.getCursorPosition()]` will return the item at the
     * current cursor position NB: This is relative positioning (i.e. if a
     * submenu is currently being displayed, menu[1] will return item 1 in
     * the current menu)
     * @return `MenuItem` - item at `position`
     */
    MenuItem* operator[](const uint8_t position) {
        return items[position];
    }

  protected:
    uint8_t itemsCount() {
        uint8_t i = 0;
        while (items[i] != nullptr) {
            i++;
        }
        return i;
    }
    /**
     * @brief Move cursor to specified position.
     */
    void setCursor(DisplayInterface* display, uint8_t position) {
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
    void draw(DisplayInterface* display) {
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
    bool process(LcdMenu* menu, const unsigned char command);
    /**
     * Execute an "up press" on menu
     * When edit mode is enabled, this action is skipped
     */
    void up(DisplayInterface* display) {
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
    /**
     * Execute a "down press" on menu
     * When edit mode is enabled, this action is skipped
     */
    void down(DisplayInterface* display) {
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
    /**
     * @brief Reset the screen to initial state.
     */
    void reset(DisplayInterface* display) {
        cursor = 0;
        view = 0;
        draw(display);
    }
};

#define MENU_SCREEN(screen, items, ...)         \
    extern MenuItem* items[];                   \
    extern MenuScreen* screen;                  \
    MenuItem* items[] = {__VA_ARGS__, nullptr}; \
    MenuScreen* screen = new MenuScreen(items)
