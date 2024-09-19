#pragma once

#include "LcdMenu.h"
#include "display/DisplayInterface.h"
#include "utils/constants.h"
#include <MenuItem.h>
#include <utils/utils.h>

/**
 * The MenuScreen class contains all fields and methods to manipulate the menu
 * items.
 */
class MenuScreen {
    friend LcdMenu;

  private:
    /**
     * Backreference to the previous screen.
     */
    MenuScreen* parent = NULL;
    /**
     * The menu items to be displayed on screen.
     */
    MenuItem** items = NULL;
    /**
     * Cursor position.
     */
    uint8_t cursor = 0;
    /**
     * First visible item's position in the menu array
     */
    uint8_t view = 0;

  public:
    /**
     * # Constructor
     */
    MenuScreen(MenuItem** items) : items(items) {}
    /**
     * Set new paren screen.
     */
    void setParent(MenuScreen* parent) {
        this->parent = parent;
    }
    /**
     * Get current cursor position.
     */
    uint8_t getCursor() {
        return cursor;
    }
    /**
     * Get a `MenuItem` at position.
     * @return `MenuItem` - item at `position`
     */
    MenuItem* getItemAt(uint8_t position) {
        return items[position];
    }
    /**
     * Get a `MenuItem` at position using operator function
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
        while(items[i] != nullptr) {
            i++;
        }
        return i;
    }
    /**
     * Move cursor to specified position.
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
        for (uint8_t i = 0; i < display->getMaxRows(); i++) {
            MenuItem* item = this->items[view + i];
            if (item == nullptr) {
                return;
            }
            item->draw(display, i);
        }
        if (view == 0) {
            display->clearUpIndicator();
        } else {
            display->drawUpIndicator();
        }
        if (items[view + display->getMaxRows()] != nullptr) {
            display->drawDownIndicator();
        } else {
            display->clearDownIndicator();
        }
        display->moveCursor(cursor - view);
        display->drawCursor();  // In case if currentPosition was not changed between screens
    }
    bool process(MenuItem::Context context) {
        if (items[cursor]->process(context)) {
            return true;
        }
        switch (context.command) {
            case UP: return up(context);
            case DOWN: return down(context);
            case BACK: return back(context);
            default: return false;
        }
    }
    /**
     * Execute an "up press" on menu
     * When edit mode is enabled, this action is skipped
     */
    bool up(MenuItem::Context context) {
        if (cursor == 0) {
            return false;
        }
        DisplayInterface* display = context.display;
        cursor--;
        if (cursor < view) {
            view--;
            draw(display);
        } else {
            display->moveCursor(cursor - view);
        }
        return true;
    }
    /**
     * Execute a "down press" on menu
     * When edit mode is enabled, this action is skipped
     */
    bool down(MenuItem::Context context) {
        if (cursor == itemsCount - 1) {
            return false;
        }
        DisplayInterface* display = context.display;
        cursor++;
        if (cursor > view + display->getMaxRows() - 1) {
            view++;
            draw(display);
        } else {
            display->moveCursor(cursor - view);
        }
        return true;
    }
    /**
     * Execute a "backpress" action on menu.
     *
     * Navigates up once.
     */
    bool back(MenuItem::Context context) {
        if (parent != NULL) {
            context.menu->setCurrentScreen(parent);
        }
        return true;
    }

    void reset(DisplayInterface* display) {
        cursor = 0;
        view = 0;
        draw(display);
    }
};
