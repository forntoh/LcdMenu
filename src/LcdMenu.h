/*
  LcdMenu.h - Main include file for the LcdMenu Library

  MIT License

  Copyright (c) 2020-2023 Forntoh Thomas

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

    bool isEditModeEnabled = false;
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
     * Column location of Blinker
     */
    uint8_t blinkerPosition = 0;
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
        lcd.update(currentMenuTable, cursorPosition, blinkerPosition, top,
                   bottom, isEditModeEnabled);
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
        if (lcd.isAtTheStart() || isEditModeEnabled) return;
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
        if (lcd.isAtTheEnd() || isEditModeEnabled) return;
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
        switch (item->getType()) {
            //
            // switch the menu to the selected sub menu
            //
            case MENU_ITEM_SUB_MENU: {
                //
                // check if there is a sub menu
                //
                if (item->getSubMenu() == NULL) return;
                currentMenuTable = item->getSubMenu();
                //
                // display the sub menu
                //
                reset(false);
                break;
            }
#ifdef ItemCommand_H
            //
            // execute the menu item's function
            //
            case MENU_ITEM_COMMAND: {
                //
                // execute the menu item's function
                //
                if (item->getCallback() != NULL) (item->getCallback())();
                //
                // display the menu again
                //
                update();
                break;
            }
#endif
#ifdef ItemToggle_H
            case MENU_ITEM_TOGGLE: {
                //
                // toggle the value of isOn
                //
                item->setIsOn(!item->isOn());
                //
                // execute the menu item's function
                //
                if (item->getCallbackInt() != NULL)
                    (item->getCallbackInt())(item->isOn());
                //
                // display the menu again
                //
                update();
                break;
            }
#endif
#ifdef ItemInput_H
            case MENU_ITEM_INPUT: {
                //
                // enter editmode
                //
                if (!isInEditMode()) {
                    isEditModeEnabled = true;
                    // blinker will be drawn
                    lcd.drawCursor();
                }
                break;
            }
#endif
            case MENU_ITEM_PROGRESS:
            case MENU_ITEM_LIST: {
                //
                // execute the menu item's function
                //
                if (!isInEditMode()) {
                    isEditModeEnabled = true;
                    lcd.drawCursor();
                }
                break;
            }
        }
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
        if (isInEditMode()) switch (item->getType()) {
#ifdef ItemInput_H
                case MENU_ITEM_INPUT:
                    // Disable edit mode
                    isEditModeEnabled = false;
                    update();
                    // Execute callback function
                    if (item->getCallbackStr() != NULL)
                        (item->getCallbackStr())(item->getValue());
                    // Interrupt going back to parent menu
                    return;
#endif
#if defined(ItemProgress_H) || defined(ItemList_H)
                case MENU_ITEM_LIST:
                case MENU_ITEM_PROGRESS:
                    // Disable edit mode
                    isEditModeEnabled = false;
                    update();
                    // Execute callback function
                    if (item->getCallbackInt() != NULL)
                        (item->getCallbackInt())(item->getItemIndex());
                    // Interrupt going back to parent menu
                    return;
#endif
                default:
                    break;
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
        if (isInEditMode() && isCharPickerActive) return;
        //
        MenuItem* item = currentMenuTable[cursorPosition];
        //
        // get the type of the currently displayed menu
        //
#ifdef ItemList_H
        uint8_t previousIndex = item->getItemIndex();
#endif
        switch (item->getType()) {
#ifdef ItemList_H
            case MENU_ITEM_LIST: {
                if (isInEditMode()) {
                    item->setItemIndex(item->getItemIndex() - 1);
                    if (previousIndex != item->getItemIndex()) update();
                    // Log
                    printCmd(F("LEFT"),
                             item->getItems()[item->getItemIndex()].c_str());
                }
                break;
            }
#endif
#ifdef ItemInput_H
            case MENU_ITEM_INPUT: {
                blinkerPosition--;
                lcd.resetBlinker();
                // Log
                printCmd(F("LEFT"),
                         item->getValue()[blinkerPosition -
                                          (strlen(item->getText()) + 2)]);
                break;
            }
#endif
#ifdef ItemProgress_H
            case MENU_ITEM_PROGRESS: {
                if (isInEditMode() && item->getItemIndex() > MIN_PROGRESS) {
                    item->decrement();
                    update();
                    // Log
                    printCmd(F("LEFT"), item->getValue());
                }
                break;
            }
#endif
            default:
                // Log
                printCmd(F("LEFT"));
                break;
        }
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
        if (isInEditMode() && isCharPickerActive) return;
        //
        MenuItem* item = currentMenuTable[cursorPosition];
        //
        // get the type of the currently displayed menu
        //
        switch (item->getType()) {
#ifdef ItemList_H
            case MENU_ITEM_LIST: {
                if (isInEditMode()) {
                    item->setItemIndex((item->getItemIndex() + 1) %
                                       item->getItemCount());
                    // constrain(item->itemIndex + 1, 0, item->itemCount - 1);
                    update();
                    // Log
                    printCmd(F("RIGHT"),
                             item->getItems()[item->getItemIndex()].c_str());
                }
                break;
            }
#endif
#ifdef ItemInput_H
            case MENU_ITEM_INPUT: {
                blinkerPosition++;
                lcd.resetBlinker();
                // Log
                printCmd(F("RIGHT"),
                         item->getValue()[blinkerPosition -
                                          (strlen(item->getText()) + 2)]);
                break;
            }
#endif
#ifdef ItemProgress_H
            case MENU_ITEM_PROGRESS: {
                if (isInEditMode() && item->getItemIndex() < MAX_PROGRESS) {
                    item->increment();
                    update();
                    // Log
                    printCmd(F("RIGHT"), item->getValue());
                }
                break;
            }
#endif
            default:
                // Log
                printCmd(F("RIGHT"));
                break;
        }
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
        uint8_t p = blinkerPosition - (strlen(item->getText()) + 2) - 1;
        remove(item->getValue(), p, 1);
        // Log
        printCmd(F("BACKSPACE"), item->getValue());
        //
        blinkerPosition--;
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
        if (item->getType() != MENU_ITEM_INPUT || !isEditModeEnabled) return;
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
        if (blinkerPosition < ub) {
            char start[10];
            char end[10];
            char* joined = new char[maxCols - lb];
            substring(item->getValue(), 0, blinkerPosition - lb, start);
            substring(item->getValue(), blinkerPosition + 1 - lb, length, end);
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
        blinkerPosition++;
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
        blinkerPosition = 0;
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
     * To know weather the menu is in edit mode or not
     * @return `bool` - isEditModeEnabled
     */
    bool isInEditMode() { return isEditModeEnabled; }
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
