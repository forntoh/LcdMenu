/*
  LcdMenu.h - Main include file for the LcdMenu Library

  MIT License

  Copyright (c) 2020-2024 Forntoh Thomas

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/
#pragma once

#include "display/DisplayInterface.h"
#include "utils/constants.h"
#include <MenuItem.h>
#include <utils/utils.h>

/**
 * The LcdMenu class contains all fields and methods to manipulate the menu
 * items.
 */
class LcdMenu {
  private:
    uint8_t cursorPosition = 1;
    uint8_t previousCursorPosition = 1;
    /**
     * First visible item's position in the menu array
     */
    uint8_t top = 1;
    uint8_t previousTop = 1;
    /**
     * Last visible item's position in the menu array
     */
    uint8_t bottom = 0;
    uint8_t previousBottom = 0;
    uint8_t maxRows;
    uint8_t maxCols;
    /**
     * Array of menu items
     */
    MenuItem** currentMenuTable = NULL;
    /**
     * Determines whether the screen should be updated after an action. Set it
     * to `false` when you want to display any other content on the screen then
     * set it back to `true` to show the menu.
     */
    bool enableUpdate = true;
    /**
     * Reset the display
     * @param isHistoryAvailable indicates if there is a previous position
     */
    void reset(boolean isHistoryAvailable) {
        if (isHistoryAvailable) {
            cursorPosition = previousCursorPosition;
            top = previousTop;
            bottom = previousBottom;
        } else {
            previousCursorPosition = cursorPosition;
            previousTop = top;
            previousBottom = bottom;

            cursorPosition = 1;
            top = 1;
            bottom = maxRows;
        }
        lcd.clear();
        drawMenu();
        lcd.moveCursor(constrain(cursorPosition - top, 0, maxRows - 1));
        lcd.drawCursor();  // In case if currentPosition was not changed between screens
    }

    /*
     * Draw the cursor
     */
    void updateOnlyCursor() {
        if (!enableUpdate) {
            return;
        }
        lcd.moveCursor(constrain(cursorPosition - top, 0, maxRows - 1));
    }

    void drawMenu() {
        for (uint8_t i = top; i <= bottom; i++) {
            MenuItem* item = currentMenuTable[i];
            if (currentMenuTable[i]->getType() == MENU_ITEM_END_OF_MENU) {
                return;
            }
            item->draw(&lcd, i - top);
        }
        if (isAtTheStart(top)) {
            lcd.clearUpIndicator();
        } else {
            lcd.drawUpIndicator();
        }
        if (isAtTheEnd(bottom)) {
            lcd.clearDownIndicator();
        } else {
            lcd.drawDownIndicator();
        }
    }

  public:
    /**
     * ## Public Fields
     */
    /**
     * Display Interface
     */
    DisplayInterface& lcd;

    /**
     * # Constructor
     */
    LcdMenu(DisplayInterface& display) : lcd(display) {
        bottom = lcd.getMaxRows();
        maxRows = lcd.getMaxRows();
        maxCols = lcd.getMaxCols();
    }

    void initialize(MenuItem* menu[]) {
        lcd.begin();
        currentMenuTable = menu;
        drawMenu();
        lcd.drawCursor();
    }

    /*
     * Draw the menu items and cursor
     */
    void update() {
        if (!enableUpdate) {
            return;
        }
        drawMenu();
        lcd.drawCursor();
    }

    bool process(const unsigned char c) {
        MenuItem::Context context{this, &lcd, c};
        if (currentMenuTable[cursorPosition]->process(context)) {
            return true;
        }
        switch (c) {
            case UP: return up();
            case DOWN: return down();
            case ENTER: return enter();
            case BACK: return back();
            default: return false;
        }
    };

    /**
     * When you want to display any other content on the screen then
     * call this function then display your content, later call
     * `show()` to show the menu
     */
    void hide() {
        enableUpdate = false;
        lcd.clear();
    }
    /**
     * Show the menu
     */
    void show() {
        enableUpdate = true;
        update();
    }
    /**
     * @brief Checks if the given position is at the start.
     * @param position The position to check.
     * @return true if the position is at the start (i.e., equal to 1),
     *         false otherwise.
     */
    inline bool isAtTheStart(uint8_t position) {
        return position == 1;
    }
    /**
     * @brief Checks if the specified position is at the end of the menu.
     * @param position The index of the item to check.
     * @return true if the next item is the end of the menu; false otherwise.
     */
    inline bool isAtTheEnd(uint8_t position) {
        return currentMenuTable[position + 1]->getType() == MENU_ITEM_END_OF_MENU;
    }
    /**
     * Get the current cursor position
     * @return `cursorPosition` e.g. 1, 2, 3...
     */
    uint8_t getCursorPosition() { return this->cursorPosition; }
    /**
     * Set the current cursor position
     * @param position
     */
    void setCursorPosition(uint8_t position) {
        uint8_t bottom = position;
        bool isNotEnd = false;

        do {
            isNotEnd = currentMenuTable[++bottom]->getType() != MENU_ITEM_END_OF_MENU && bottom < maxRows + 20;
        } while (isNotEnd);

        uint8_t max = maxRows - 1;

        this->cursorPosition = position + 1;
        this->bottom = constrain(cursorPosition + max, max, bottom - 1);
        this->top = this->bottom - max;
    }
    /**
     * Check if currently displayed menu is a sub menu.
     */
    bool isSubMenu() {
        byte menuItemType = currentMenuTable[0]->getType();
        return menuItemType == MENU_ITEM_SUB_MENU_HEADER;
    }
    /**
     * Get a `MenuItem` at position
     * @return `MenuItem` - item at `position`
     */
    MenuItem* getItemAt(uint8_t position) { return currentMenuTable[position]; }
    /**
     * Get a `MenuItem` at position using operator function
     * e.g `menu[menu.getCursorPosition()]` will return the item at the
     * current cursor position NB: This is relative positioning (i.e. if a
     * submenu is currently being displayed, menu[1] will return item 1 in
     * the current menu)
     * @return `MenuItem` - item at `position`
     */
    MenuItem* operator[](const uint8_t position) {
        return currentMenuTable[position];
    }

  protected:
    /**
     * Execute an "up press" on menu
     * When edit mode is enabled, this action is skipped
     */
    bool up() {
        //
        // determine if cursor ia at start of menu items
        //
        if (isAtTheStart(cursorPosition)) {
            return false;
        }
        cursorPosition--;
        // Log
        printCmd(F("UP"), cursorPosition);
        //
        // determine if cursor is at the top of the screen
        //
        if (cursorPosition < top) {
            //
            // scroll up once
            //
            top--;
            bottom--;
            update();
        } else {
            updateOnlyCursor();
        }
        return true;
    }
    /**
     * Execute a "down press" on menu
     * When edit mode is enabled, this action is skipped
     */
    bool down() {
        //
        // determine if cursor has passed the end
        //
        if (isAtTheEnd(cursorPosition)) {
            return false;
        }
        cursorPosition++;
        // Log
        printCmd(F("DOWN"), cursorPosition);
        //
        // determine if cursor is at the bottom of the screen
        //
        if (cursorPosition > bottom) {
            //
            // scroll down once
            //
            top++;
            bottom++;
            update();
        } else {
            updateOnlyCursor();
        }
        return true;
    }
    /**
     * Execute an "enter" action on menu.
     *
     * It does the following depending on the type of the current menu item:
     *
     * - Open a sub menu.
     * - Execute a callback action.
     * - Toggle the state of an item.
     */
    bool enter() {
        MenuItem* item = currentMenuTable[cursorPosition];
        // Log
        printCmd(F("ENTER"), item->getType());
        //
        // check if there is a sub menu
        //
        if (item->getSubMenu() != NULL) {
            //
            // switch the menu to the selected sub menu
            //
            currentMenuTable = item->getSubMenu();
            //
            // display the sub menu
            //
            reset(false);
        }
        return true;
    }
    /**
     * Execute a "backpress" action on menu.
     *
     * Navigates up once.
     */
    bool back() {
        // Log
        printCmd(F("BACK"));
        //
        // check if this is a sub menu, if so go back to its parent
        //
        if (isSubMenu()) {
            currentMenuTable = currentMenuTable[0]->getSubMenu();
            reset(true);
        }
        return true;
    }
};
