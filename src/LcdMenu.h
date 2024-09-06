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

#include <MenuItem.h>
#include <utils/utils.h>

#include "interface/DisplayInterface.h"

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
     * Will prevent left and right movement when in edit mode and character
     * picker is active
     */
    bool isCharPickerActive = false;
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
        update();
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
        update();
    }
    /*
     * Draw the menu items and cursor
     */
    void update() {
        if (!enableUpdate) return;
        lcd.update(cursorPosition, top, bottom);
        drawMenu();
        byte menuType = currentMenuTable[cursorPosition - 1]->getType();
        lcd.drawCursor(menuType == MENU_ITEM_INPUT);
    }

    bool isAtTheStart() {
        byte menuType = currentMenuTable[cursorPosition - 1]->getType();
        return menuType == MENU_ITEM_MAIN_MENU_HEADER || menuType == MENU_ITEM_SUB_MENU_HEADER;
    }

    bool isAtTheEnd() { return isAtTheEnd(cursorPosition); }

    bool isAtTheEnd(uint8_t position) {
        return currentMenuTable[position + 1]->getType() == MENU_ITEM_END_OF_MENU;
    }

    void drawMenu() {
        lcd.clear();
        for (uint8_t i = top; i <= bottom; i++) {
            MenuItem* item = currentMenuTable[i];
            if (currentMenuTable[i]->getType() == MENU_ITEM_END_OF_MENU) {
                break;
            }
            lcd.setCursor(1, map(i, top, bottom, 0, maxRows - 1));
            item->draw(&lcd);
        }
        if (top == 1 && !isAtTheEnd(bottom)) {
            lcd.drawDownIndicator(maxCols - 1, maxRows - 1);
        } else if (!isAtTheStart() && !isAtTheEnd()) {
            lcd.drawDownIndicator(maxCols - 1, maxRows - 1);
            lcd.drawUpIndicator(maxCols - 1, 0);
        } else if (isAtTheEnd() && top != 1) {
            lcd.drawUpIndicator(maxCols - 1, 0);
        }
    }

    /**
     * Reset the display
     */
    void resetMenu() { this->reset(false); }
    /**
     * Execute an "up press" on menu
     * When edit mode is enabled, this action is skipped
     */
    void up() {
        //
        // determine if cursor ia at start of menu items
        //
        if (isAtTheStart() || lcd.getEditModeEnabled()) return;
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
        }
        update();
    }
    /**
     * Execute a "down press" on menu
     * When edit mode is enabled, this action is skipped
     */
    void down() {
        //
        // determine if cursor has passed the end
        //
        if (isAtTheEnd() || lcd.getEditModeEnabled()) return;
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
        }
        update();
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
    void enter() {
        MenuItem* item = currentMenuTable[cursorPosition];
        // Log
        printCmd(F("ENTER"), item->getType());
        //
        // determine the type of menu entry, then execute it
        //
        if (item->getType() == MENU_ITEM_SUB_MENU) {
            //
            // switch the menu to the selected sub menu
            //
            //
            // check if there is a sub menu
            //
            if (item->getSubMenu() == NULL) return;
            currentMenuTable = item->getSubMenu();
            //
            // display the sub menu
            //
            reset(false);
            return;
        }
        if (item->enter(&lcd)) update();
    }
    /**
     * Execute a "backpress" action on menu.
     *
     * Navigates up once.
     */
    void back() {
        // Log
        printCmd(F("BACK"));
        MenuItem* item = currentMenuTable[cursorPosition];
        //
        // Back action different when on ItemInput
        //
        if (lcd.getEditModeEnabled()) {
            if (item->back(&lcd)) {
                update();
            }
            return;
        }
        //
        // check if this is a sub menu, if so go back to its parent
        //
        if (isSubMenu()) {
            currentMenuTable = currentMenuTable[0]->getSubMenu();
            reset(true);
        }
    }
    /**
     * Execute a "left press" on menu
     *
     * *NB: Works only for `ItemInput` and `ItemList` types*
     *
     * Moves the cursor one step to the left.
     */
    void left() {
        //
        if (lcd.getEditModeEnabled() && isCharPickerActive) return;
        //
        MenuItem* item = currentMenuTable[cursorPosition];
        if (item->left(&lcd)) update();
    }
    /**
     * Execute a "right press" on menu
     *
     * *NB: Works only for `ItemInput` and `ItemList` types*
     *
     * Moves the cursor one step to the right.
     */
    void right() {
        //
        // Is the menu in edit mode and is the character picker active?
        //
        if (lcd.getEditModeEnabled() && isCharPickerActive) return;
        //
        MenuItem* item = currentMenuTable[cursorPosition];
        if (item->right(&lcd)) update();
    }
#ifdef ItemInput_H
    /**
     * Execute a "backspace cmd" on menu
     *
     * *NB: Works only for `ItemInput` type*
     *
     * Removes the character at the current cursor position.
     */
    void backspace() {
        MenuItem* item = currentMenuTable[cursorPosition];
        //
        if (item->getType() != MENU_ITEM_INPUT) return;
        //
        uint8_t p = lcd.blinkerPosition - (strlen(item->getText()) + 2) - 1;
        remove(item->getValue(), p, 1);
        // Log
        printCmd(F("BACKSPACE"), item->getValue());
        //
        lcd.blinkerPosition--;
        update();
    }
    /**
     * Display text at the cursor position
     * used for `Input` type menu items
     * @param character character to append
     */
    void type(const char character) {
        MenuItem* item = currentMenuTable[cursorPosition];
        //
        if (item->getType() != MENU_ITEM_INPUT || !lcd.getEditModeEnabled())
            return;
        //
        // calculate lower and upper bound
        //
        uint8_t length = strlen(item->getValue());
        uint8_t lb = strlen(item->getText()) + 2;
        uint8_t ub = lb + length;
        ub = constrain(ub, lb, maxCols - 2);
        //
        // update text
        //
        if (lcd.blinkerPosition < ub) {
            char start[10];
            char end[10];
            char* joined = new char[maxCols - lb];
            substring(item->getValue(), 0, lcd.blinkerPosition - lb, start);
            substring(item->getValue(), lcd.blinkerPosition + 1 - lb, length,
                      end);
            concat(start, character, end, joined);
            item->setValue(joined);
        } else {
            char* buf = new char[length + 2];
            concat(item->getValue(), character, buf);
            item->setValue(buf);
        }
        //
        isCharPickerActive = false;
        //
        // update blinker position
        //
        lcd.blinkerPosition++;
        //
        // repaint menu
        //
        update();
        // Log
        printCmd(F("TYPE-CHAR"), character);
    }
    void drawChar(char c) { isCharPickerActive = lcd.drawChar(c); }
    /**
     * Clear the value of the input field
     */
    void clear() {
        MenuItem* item = currentMenuTable[cursorPosition];
        //
        if (item->getType() != MENU_ITEM_INPUT) return;
        //
        // set the value
        //
        item->setValue((char*)"");
        // Log
        printCmd(F("CLEAR"), item->getValue());
        //
        // update blinker position
        //
        lcd.blinkerPosition = 0;
        //
        // repaint menu
        //
        update();
    }
#endif
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
            isNotEnd = currentMenuTable[++bottom]->getType() !=
                           MENU_ITEM_END_OF_MENU &&
                       bottom < maxRows + 20;
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
};
