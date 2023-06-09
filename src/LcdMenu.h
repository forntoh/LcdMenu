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

#ifndef USE_STANDARD_LCD
#include <LiquidCrystal_I2C.h>
#else
#include <LiquidCrystal.h>
#endif

#include <MenuItem.h>

/**
 * The LcdMenu class contains all fields and methods to manipulate the menu
 * items.
 */
class LcdMenu {
   private:
    /**
     * ---
     *
     * ## Private Fields
     */

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
     * Rows on the LCD Display
     */
    uint8_t maxRows;
    /**
     * Columns on the LCD Display
     */
    uint8_t maxCols;
    /**
     * Colum location of Blinker
     */
    uint8_t blinkerPosition = 0;
    /**
     * Array of menu items
     */
    MenuItem** currentMenuTable = NULL;
    /**
     * Down arrow (↓)
     */
    byte downArrow[8] = {
        0b00100,  //   *
        0b00100,  //   *
        0b00100,  //   *
        0b00100,  //   *
        0b00100,  //   *
        0b10101,  // * * *
        0b01110,  //  ***
        0b00100   //   *
    };
    /**
     * Up arrow (↑)
     */
    byte upArrow[8] = {
        0b00100,  //   *
        0b01110,  //  ***
        0b10101,  // * * *
        0b00100,  //   *
        0b00100,  //   *
        0b00100,  //   *
        0b00100,  //   *
        0b00100   //   *
    };
    /**
     * Cursor icon. Defaults to right arrow (→).
     */
    uint8_t cursorIcon = 0x7E;
    /**
     * Determines whether the screen should be updated after an action. Set it
     * to `false` when you want to display any other content on the screen then
     * set it back to `true` to show the menu.
     */
    bool enableUpdate = true;

    /**
     * ## Private Methods
     */

    /**
     * Draws the cursor
     */
    void drawCursor() {
        //
        // Erases current cursor
        //
        for (uint8_t x = 0; x < maxRows; x++) {
            lcd->setCursor(0, x);
            lcd->print(" ");
        }
        //
        // draws a new cursor at [line]
        //
        uint8_t line = constrain(cursorPosition - top, 0, maxRows - 1);
        lcd->setCursor(0, line);
        lcd->write(cursorIcon);
#ifdef ItemInput_H
        //
        // If cursor is at MENU_ITEM_INPUT enable blinking
        //
        MenuItem* item = currentMenuTable[cursorPosition];
        if (item->getType() == MENU_ITEM_INPUT) {
            resetBlinker();
            if (isEditModeEnabled) {
                lcd->blink();
                return;
            }
        }
#endif
        lcd->noBlink();
    }
    /**
     * Draw the menu items with up and down indicators
     */
    void drawMenu() {
        lcd->clear();
        //
        // print the menu items
        //
        for (uint8_t i = top; i <= bottom; i++) {
            MenuItem* item = currentMenuTable[i];
            lcd->setCursor(1, map(i, top, bottom, 0, maxRows - 1));
            if (currentMenuTable[i]->getType() != MENU_ITEM_END_OF_MENU) {
                lcd->print(item->getText());
            }
            //
            // determine the type of item
            //
            switch (item->getType()) {
#ifdef ItemToggle_H
                case MENU_ITEM_TOGGLE:
                    //
                    // append textOn or textOff depending on the state
                    //
                    lcd->print(":");
                    lcd->print(item->isOn() ? item->getTextOn()
                                            : item->getTextOff());
                    break;
#endif
#ifdef ItemInput_H
                case MENU_ITEM_INPUT:
                    //
                    // append the value of the input
                    //
                    lcd->print(":");
                    lcd->print(item->getValue().substring(
                        0, maxCols - strlen(item->getText()) - 2));
                    break;
#endif
#ifdef ItemList_H
                case MENU_ITEM_LIST:
                    //
                    // append the value of the item at current list position
                    //
                    lcd->print(":");
                    lcd->print(item->getItems()[item->getItemIndex()].substring(
                        0, maxCols - strlen(item->getText()) - 2));
                    break;
#endif
                default:
                    break;
            }
            // if we reached the end of menu, stop
            if (currentMenuTable[i]->getType() == MENU_ITEM_END_OF_MENU) break;
        }
        //
        // determine if cursor is at the top
        //
        if (top == 1) {
            //
            // Print the down arrow only
            //
            lcd->setCursor(maxCols - 1, maxRows - 1);
            lcd->write(byte(1));
        } else if (!isAtTheStart() && !isAtTheEnd()) {
            //
            // Print the down arrow
            //
            lcd->setCursor(maxCols - 1, maxRows - 1);
            lcd->write(byte(1));
            //
            // Print the up arrow
            //
            lcd->setCursor(maxCols - 1, 0);
            lcd->write(byte(0));
        } else if (isAtTheEnd()) {
            //
            // Print the up arrow only
            //
            lcd->setCursor(maxCols - 1, 0);
            lcd->write(byte(0));
        }
    }
    /**
     * Check if the cursor is at the start of the menu items
     * @return true : `boolean` if it is at the start
     */
    boolean isAtTheStart() {
        byte menuType = currentMenuTable[cursorPosition - 1]->getType();
        return menuType == MENU_ITEM_MAIN_MENU_HEADER ||
               menuType == MENU_ITEM_SUB_MENU_HEADER;
    }
    /**
     * Check if the cursor is at the end of the menu items
     * @return true : `boolean` if it is at the end
     */
    boolean isAtTheEnd() {
        return currentMenuTable[cursorPosition + 1]->getType() ==
               MENU_ITEM_END_OF_MENU;
    }
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
#ifdef ItemInput_H
    /**
     * Calculate and set the new blinker position
     */
    void resetBlinker() {
        //
        // calculate lower and upper bound
        //
        uint8_t lb = strlen(currentMenuTable[cursorPosition]->getText()) + 2;
        uint8_t ub = lb + currentMenuTable[cursorPosition]->getValue().length();
        ub = constrain(ub, lb, maxCols - 2);
        //
        // set cursor position
        //
        blinkerPosition = constrain(blinkerPosition, lb, ub);
        lcd->setCursor(blinkerPosition, cursorPosition - top);
    }
#endif

   public:
    /**
     * ## Public Fields
     */

    /**
     * Time when the timer started in milliseconds
     */
    unsigned long startTime = 0;
    /**
     * How long should the timer delay in milliseconds
     */
    unsigned int delay = 0;
    /**
     * LCD Display
     */
#ifndef USE_STANDARD_LCD
    LiquidCrystal_I2C* lcd = NULL;
#else
    LiquidCrystal* lcd = NULL;
#endif

    /**
     * # Constructor
     */

    /**
     * Constructor for the LcdMenu class
     * @param maxRows rows on lcd display e.g. 4
     * @param maxCols columns on lcd display e.g. 20
     * @return new `LcdMenu` object
     */
    LcdMenu(uint8_t maxRows, uint8_t maxCols)
        : bottom(maxRows), maxRows(maxRows), maxCols(maxCols) {}

    /**
     * ## Public Methods
     */

    /**
     * Call this function in `setup()` to initialize the LCD and the custom
     * characters used as up and down arrows
     * @param lcd_Addr address of the LCD on the I2C bus (default 0x27)
     * @param menu menu to display
     */
    void setupLcdWithMenu(
#ifndef USE_STANDARD_LCD
        uint8_t lcd_Addr, MenuItem** menu) {
        lcd = new LiquidCrystal_I2C(lcd_Addr, maxCols, maxRows);
        lcd->init();
        lcd->backlight();
#else
        uint8_t rs, uint8_t en, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
        MenuItem** menu) {
        this->lcd = new LiquidCrystal(rs, en, d0, d1, d2, d3);
        this->lcd->begin(maxCols, maxRows);
#endif
        lcd->clear();
        lcd->createChar(0, upArrow);
        lcd->createChar(1, downArrow);
        this->currentMenuTable = menu;
        update();
    }
    /*
     * Draw the menu items and cursor
     */
    void update() {
        if (!enableUpdate) return;
        drawMenu();
        drawCursor();
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
        if (isAtTheStart() || isEditModeEnabled) return;
        cursorPosition--;
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
        if (isAtTheEnd() || isEditModeEnabled) return;
        cursorPosition++;
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
#ifdef ItemList_H
            case MENU_ITEM_LIST: {
                //
                // execute the menu item's function
                //
                if (item->getCallbackInt() != NULL)
                    (item->getCallbackInt())(item->getItemIndex());
                break;
            }
#endif
        }
    }
    /**
     * Execute a "backpress" action on menu.
     *
     * Navigates up once.
     */
    void back() {
#ifdef ItemInput_H
        MenuItem* item = currentMenuTable[cursorPosition];
        //
        // Back action different when on ItemInput
        //
        if (item->getType() == MENU_ITEM_INPUT && isInEditMode()) {
            // Disable edit mode
            isEditModeEnabled = false;
            update();
            // Execute callback function
            if (item->getCallbackStr() != NULL)
                (item->getCallbackStr())(item->getValue());
            // Interrupt going back to parent menu
            return;
        }
#endif
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
                item->setItemIndex(constrain(item->getItemIndex() - 1, 0,
                                             item->getItemCount() - 1));
                if (previousIndex != item->getItemIndex()) update();
                break;
            }
#endif
#ifdef ItemInput_H
            case MENU_ITEM_INPUT: {
                blinkerPosition--;
                resetBlinker();
                break;
            }
#endif
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
                item->setItemIndex((item->getItemIndex() + 1) %
                                   item->getItemCount());
                // constrain(item->itemIndex + 1, 0, item->itemCount - 1);
                update();
                break;
            }
#endif
#ifdef ItemInput_H
            case MENU_ITEM_INPUT: {
                blinkerPosition++;
                resetBlinker();
                break;
            }
#endif
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
        item->getValue().remove(p, 1);
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
        uint8_t lb = strlen(item->getText()) + 2;
        uint8_t ub = lb + item->getValue().length();
        ub = constrain(ub, lb, maxCols - 2);
        //
        // update text
        //
        if (blinkerPosition < ub) {
            String start = item->getValue().substring(0, blinkerPosition - lb);
            String end = item->getValue().substring(blinkerPosition + 1 - lb,
                                                    item->getValue().length());
            item->setValue(start + character + end);
        } else {
            item->setValue(item->getValue().concat(character) + "");
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
    }
    /**
     * Draw a character on the display
     * used for `Input` type menu items.
     * @param c character to draw
     */
    void drawChar(char c) {
        MenuItem* item = currentMenuTable[cursorPosition];
        //
        if (item->getType() != MENU_ITEM_INPUT || !isEditModeEnabled) return;
        //
        // draw the character without updating the menu item
        //
        uint8_t line = constrain(cursorPosition - top, 0, maxRows - 1);
        lcd->setCursor(blinkerPosition, line);
        lcd->print(c);
        resetBlinker();
        //
        isCharPickerActive = true;
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
        item->setValue("");
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
     * Set the character used to visualize the cursor.
     * @param newIcon character to display
     */
    void setCursorIcon(uint8_t newIcon) {
        cursorIcon = newIcon;
        drawCursor();
    }
    /**
     * When you want to display any other content on the screen then
     * call this function then display your content, later call
     * `show()` to show the menu
     */
    void hide() {
        enableUpdate = false;
        lcd->clear();
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
        this->cursorPosition = position;
    }
    /**
     * Show a message at the bottom of the screen
     * @param message message to display
     * @param duration how long to display the message
     */
    void displayNotification(char* message, unsigned int duration) {
        //
        // Calculate the position to start writing
        // (centralize text)
        //
        uint8_t centerPos = maxCols / 2 - (strlen(message) / 2);
        //
        // Set cursor position and clear lane
        //
        lcd->setCursor(0, maxRows - 1);
        lcd->print("                   ");
        lcd->setCursor(centerPos - 1, maxRows - 1);
        //
        // Draw each independent character
        //
        lcd->write(0xA5);
        for (unsigned int i = 0; i < strlen(message); i++) {
            char character = message[i];
            lcd->write(character);
        }
        lcd->write(0xA5);
        //
        // initialize the timer
        //
        delay = duration;
        startTime = millis();
    }
    /**
     * Executes any delayed task when appropriate time reaches
     */
    void updateTimer() {
        if (millis() == startTime + delay) update();
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
     * e.g `menu[menu.getCursorPosition()]` will return the item at the current
     * cursor position
     * NB: This is relative positioning (i.e. if a submenu is currently being
     * displayed, menu[1] will return item 1 in the current menu)
     * @return `MenuItem` - item at `position`
     */
    MenuItem* operator[](const uint8_t position) {
        return currentMenuTable[position];
    }
#ifdef ItemToggle_H
#ifndef USE_STANDARD_LCD
    /**
     * Toggle backlight
     */
    void toggleBacklight() {
        MenuItem* item = currentMenuTable[cursorPosition];
        if (item->getType() == MENU_ITEM_TOGGLE) {
            lcd->setBacklight(item->isOn() ? 1 : NULL);
        }
    }
#endif // USE_STANDARD_LCD
#endif // ItemToggle_H
};
