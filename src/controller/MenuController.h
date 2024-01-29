/*
  MIT License

  Copyright (c) 2020-2024 Forntoh Thomas

  Permission is hereby granted, free of charge, to any person obtaining a
  copy of this software and associated documentation files (the "Software"),
  to deal in the Software without restriction, including without limitation
  the rights to use, copy, modify, merge, publish, distribute, sublicense,
  and/or sell copies of the Software, and to permit persons to whom the
  Software is furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
  DEALINGS IN THE SOFTWARE.
*/
#ifndef MenuController_H
#define MenuController_H

#include "../MenuItem.h"
#include "../utils/utils.h"

class MenuController {
   protected:
    /**
     * Cursor position
     */
    uint8_t cursorPosition = 1;
    uint8_t previousCursorPosition = 1;
    /**
     * First visible item's position in the menu array
     */
    uint8_t top = 1;
    uint8_t previousTop = 1;
    /**
     * Edit mode
     */
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
    /**
     * Max rows for the menu
     */
    uint8_t maxRows;
    /**
     * Columns on the LCD Display
     */
    uint8_t maxCols;
    /**
     * Time when the timer started in milliseconds
     */
    unsigned long startTime = 0;
    /**
     * Column location of Blinker relative to input value
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
     * Draws the cursor
     */
    virtual void drawCursor() = 0;
    /**
     * Draw the menu items with up and down indicators
     */
    virtual void drawMenu() = 0;
    /**
     * Reset the display
     * @param isHistoryAvailable indicates if there is a previous position
     */
    void reset(boolean isHistoryAvailable);
    /**
     * Calculate and set the new blinker position
     */
    virtual void resetBlinker() = 0;
    /**
     * Turn on the display
     */
    virtual void displayOn() = 0;
    /**
     * Turn off the display
     */
    virtual void displayOff() = 0;

   public:
    /**
     * How long should the display stay on
     */
    uint16_t timeout = 10000;
    /**
     * Constructor for the LcdMenu class
     * @param maxRows rows on lcd display e.g. 4
     * @param maxCols columns on lcd display e.g. 20
     * @return new `LcdMenu` object
     */
    explicit MenuController(uint8_t maxRows, uint8_t maxCols)
        : bottom(maxRows), maxRows(maxRows), maxCols(maxCols) {}
    /**
     * Check if the cursor is at the start of the menu items
     * @return true : `boolean` if it is at the start
     */
    bool isAtTheStart();
    /**
     * Check if the cursor is at the end of the menu items
     * @return true : `boolean` if it is at the end
     */
    bool isAtTheEnd();
    /*
     * Update the menu
     */
    virtual void update() = 0;
    /**
     * Reset the display
     */
    void resetMenu();
    /**
     * Execute an "up press" on menu
     * When edit mode is enabled, this action is skipped
     */
    void up();
    /**
     * Execute a "down press" on menu
     * When edit mode is enabled, this action is skipped
     */
    void down();
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
                    drawCursor();
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
                    drawCursor();
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
                resetBlinker();
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
    } /**
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
                resetBlinker();
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
    void type(char character) {
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
            static char start[10];
            static char end[10];
            static char* joined = new char[maxCols - lb];
            substring(item->getValue(), 0, blinkerPosition - lb, start);
            substring(item->getValue(), blinkerPosition + 1 - lb, length, end);
            concat(start, character, end, joined);
            item->setValue(joined);
        } else {
            static char* buf = new char[length + 2];
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
        item->setValue(nullptr);
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
     * Draw a character on the display
     * used for `Input` type menu items.
     * @param c character to draw
     */
    virtual void drawChar(char c) = 0;
    /**
     * When you want to display any other content on the screen then
     * call this function then display your content, later call
     * `show()` to show the menu
     */
    virtual void hide() = 0;
    /**
     * Show the menu
     */
    void show();
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
        this->cursorPosition = position;
    }
    /**
     * Check if currently displayed menu is a sub menu.
     */
    bool isSubMenu();
    /**
     * Get a `MenuItem` at position
     * @return `MenuItem` - item at `position`
     */
    MenuItem* getItemAt(uint8_t position) { return currentMenuTable[position]; }
    /**
     * Get a `MenuItem` at position using operator function
     * e.g `menu[menu.getCursorPosition()]` will return the item at the current
     * cursor position
     * NB: This is relative positioning (i.e. if a submenu is currently being
     * displayed, menu[1] will return item 1 in the current menu)
     * @return `MenuItem` - item at `position`
     */
    MenuItem* operator[](const uint8_t position) {
        return currentMenuTable[position];
    }
    /**
     * Update timer and turn off display on timeout
     */
    void updateTimer();
};

#endif