/*
  LcdMenu.h - Main include file for the LcdMenu Library

  MIT License

  Copyright (c) 2020-2021 Forntoh Thomas

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

#ifndef LcdMenu_H
#define LcdMenu_H

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
    MenuItem* currentMenuTable = NULL;
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
        //
        // If cursor is at MENU_ITEM_INPUT enable blinking
        //
        MenuItem* item = &currentMenuTable[cursorPosition];
        if (item->getType() == MENU_ITEM_INPUT) {
            resetBlinker();
            lcd->blink();
        } else
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
            MenuItem* item = &currentMenuTable[i];
            lcd->setCursor(1, map(i, top, bottom, 0, maxRows - 1));
            lcd->print(item->getText());
            //
            // determine the type of item
            //
            switch (item->getType()) {
                case MENU_ITEM_TOGGLE:
                    //
                    // append textOn or textOff depending on the state
                    //
                    lcd->print(":");
                    lcd->print(item->isOn ? item->getTextOn()
                                          : item->getTextOff());
                    break;
                case MENU_ITEM_INPUT:
                    //
                    // append the value the value of the input
                    //
                    lcd->print(":");
                    lcd->print(item->value.substring(
                        0, maxCols - strlen(item->getText()) - 2));
                    break;
                default:
                    break;
            }
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
        byte menuType = currentMenuTable[cursorPosition - 1].getType();
        return menuType == MENU_ITEM_MAIN_MENU_HEADER ||
               menuType == MENU_ITEM_SUB_MENU_HEADER;
    }
    /**
     * Check if the cursor is at the end of the menu items
     * @return true : `boolean` if it is at the end
     */
    boolean isAtTheEnd() {
        return currentMenuTable[cursorPosition + 1].getType() ==
               MENU_ITEM_END_OF_MENU;
    }
    /*
     * Draw the menu items and cursor
     */
    void paint() {
        if (!enableUpdate) return;
        drawMenu();
        drawCursor();
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
        paint();
    }
    /**
     * Calculate and set the new blinker position
     */
    void resetBlinker() {
        //
        // calculate lower and upper bound
        //
        uint8_t lb = strlen(currentMenuTable[cursorPosition].getText()) + 2;
        uint8_t ub = lb + currentMenuTable[cursorPosition].value.length();
        ub = constrain(ub, lb, maxCols - 2);
        //
        // set cursor position
        //
        blinkerPosition = constrain(blinkerPosition, lb, ub);
        lcd->setCursor(blinkerPosition, cursorPosition - top);
    }

   public:
    /**
     * ## Public Fields
     */

    /**
     * Time when the toast started showing in milliseconds
     */
    unsigned long startTime = 0;
    /**
     * How long the toast should Last in milliseconds
     */
    unsigned int duration = 0;
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
        uint8_t lcd_Addr, MenuItem* menu) {
        lcd = new LiquidCrystal_I2C(lcd_Addr, maxCols, maxRows);
        lcd->init();
        lcd->backlight();
#else
        uint8_t rs, uint8_t en, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
        MenuItem* menu) {
        this->lcd = new LiquidCrystal(rs, en, d0, d1, d2, d3);
        this->lcd->begin(maxCols, maxRows);
#endif
        lcd->clear();
        lcd->createChar(0, upArrow);
        lcd->createChar(1, downArrow);
        this->currentMenuTable = menu;
        paint();
    }
    /**
     * Call this function to set sub menu items for any main menu item
     * @param position main menu item/where to place the sub menu
     * @param items    sub menu items
     */
    void setSubMenu(uint8_t position, MenuItem* items) {
        currentMenuTable[position + 1].setSubMenu(items);
        paint();
    }
    /**
     * Execute an "up press" on menu
     */
    void up() {
        //
        // determine if cursor ia at start of menu items
        //
        if (isAtTheStart()) return;
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
        paint();
    }
    /**
     * Execute a "down press" on menu
     */
    void down() {
        //
        // determine if cursor has passed the end
        //
        if (isAtTheEnd()) return;
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
        paint();
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
        MenuItem* item = &currentMenuTable[cursorPosition];
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
                paint();
                break;
            }
            case MENU_ITEM_TOGGLE: {
                //
                // toggle the value of isOn
                //
                item->isOn = !item->isOn;
                //
                // execute the menu item's function
                //
                if (item->getCallback() != NULL) (item->getCallback())();
                //
                // display the menu again
                //
                paint();
                break;
            }
            case MENU_ITEM_INPUT: {
                //
                // execute the menu item's function
                //
                if (item->getCallback() != NULL) (item->getCallback())();
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
        //
        // get the type of the currently displayed menu
        //
        byte menuItemType = currentMenuTable[0].getType();
        //
        // check if this is a sub menu, if so go back to its parent
        //
        if (menuItemType == MENU_ITEM_SUB_MENU_HEADER) {
            currentMenuTable = currentMenuTable[0].getSubMenu();
            reset(true);
        }
    }
    /**
     * Execute a "left press" on menu
     *
     * *NB: Works only for `ItemInput` type*
     *
     * Moves the cursor one step to the left.
     */
    void left() {
        blinkerPosition--;
        resetBlinker();
    }
    /**
     * Execute a "right press" on menu
     *
     * *NB: Works only for `ItemInput` type*
     *
     * Moves the cursor one step to the right.
     */
    void right() {
        blinkerPosition++;
        resetBlinker();
    }
    /**
     * Execute a "backspace cmd" on menu
     *
     * *NB: Works only for `ItemInput` type*
     *
     * Removes the character at the current cursor position.
     */
    void backspace() {
        MenuItem* item = &currentMenuTable[cursorPosition];
        //
        if (item->getType() != MENU_ITEM_INPUT) return;
        //
        uint8_t p = blinkerPosition - (strlen(item->getText()) + 2) - 1;
        item->value.remove(p, 1);
        blinkerPosition--;
        paint();
    }
    /**
     * Display text at the cursor position
     * used for `Input` type menu items
     * @param character character to append
     */
    void type(String character) {
        MenuItem* item = &currentMenuTable[cursorPosition];
        //
        if (item->getType() != MENU_ITEM_INPUT) return;
        //
        // calculate lower and upper bound
        //
        uint8_t lb = strlen(item->getText()) + 2;
        uint8_t ub = lb + item->value.length();
        ub = constrain(ub, lb, maxCols - 2);
        //
        // update text
        //
        if (blinkerPosition < ub) {
            String start = item->value.substring(0, blinkerPosition - lb);
            String end = item->value.substring(blinkerPosition - lb,
                                               item->value.length());
            item->value = start + character + end;
        } else
            item->value.concat(character);
        //
        // update blinker position
        //
        blinkerPosition++;
        //
        // repaint menu
        //
        paint();
    }
    /**
     * Clear the value of the input field
     */
    void clear() {
        MenuItem* item = &currentMenuTable[cursorPosition];
        //
        if (item->getType() != MENU_ITEM_INPUT) return;
        //
        // set the value
        //
        item->value = "";
        //
        // update blinker position
        //
        blinkerPosition = 0;
        //
        // repaint menu
        //
        paint();
    }
    /**
     * Set the character used to visualize the cursor.
     * @param newIcon character to display
     */
    void setCursorIcon(uint8_t newIcon) {
        cursorIcon = newIcon;
        drawCursor();
    }
    /**
     * When you want to display any other content on the screen then call this
     * function then display your content, later call `show()` to show
     * the menu
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
        paint();
    }
    /**
     * Get the current cursor position
     * @return `cursorPosition` e.g. 1, 2, 3...
     */
    uint8_t getCursorPosition() { return this->cursorPosition; }
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
        // Set cursor potion and clear lane
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
        this->duration = duration;
        startTime = millis();
    }
    /**
     * Executes any delayed task when appropriate time reaches
     */
    void updateTimer() {
        if (millis() == startTime + duration) paint();
    }
    /**
     * Get a `MenuItem` at position
     * @return `MenuItem` - item at `position`
     */
    MenuItem* getItemAt(uint8_t position) {
        return &currentMenuTable[position];
    }
};
#endif
