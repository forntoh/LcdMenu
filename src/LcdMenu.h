/*
  LcdMenu.h - Main include file for the LcdMenu Library

  MIT License

  Copyright (c) 2020 Forntoh Thomas

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

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

#include "MenuItem.h"

/**
 * the LcdMenu class
 */
class LcdMenu {
   private:
    /**
     * Cursor position
     */
    uint8_t cursorPosition = 1;
    /**
     * First visible item's position in the menu array
     */
    uint8_t top = 1;
    /**
     * Last visible item's position in the menu array
     */
    uint8_t bottom;
    /**
     * Rows on the LCD Display
     */
    uint8_t maxRows;
    /**
     * Columns on the LCD Display
     */
    uint8_t maxCols;
    /**
     * Array of menu items
     */
    MenuItem* currentMenuTable;
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
     * Draws the cursor
     */
    void drawCursor();
    /**
     * Draw the menu items with up and down indicators
     */
    void drawMenu();
    /**
     * Check if the cursor is at the start of the menu items
     *
     * @return true : `boolean` if it is at the start
     */
    boolean isAtTheStart();
    /**
     * Check if the cursor is at the end of the menu items
     *
     * @return true : `boolean` if it is at the end
     */
    boolean isAtTheEnd();
    /*
     * Draw the menu items and cursor
     */
    void paint();
    /**
     * Reset the display
     */
    void reset();
    /**
     * Places the cursor at end of Menu's text.
     *
     * @param item MenuItem where the cursor should be placed
     * @relatesalso MenuItem
     */
    void placeCursorAtEnd(MenuItem* item);

   public:
    /**
     * Time when the toast started showing in milliseconds
     */
    unsigned long startTime;
    /**
     * How long the toast should Last in milliseconds
     */
    unsigned int duration;
    /**
     * LCD Display
     */
    LiquidCrystal_I2C* lcd;
    /**
     * Constructor for the LcdMenu class
     *
     * @param maxRows rows on lcd display e.g. 4
     * @param maxCols columns on lcd display e.g. 20
     * @return new `LcdMenu` object
     */
    LcdMenu(uint8_t maxRows, uint8_t maxCols);
    /**
     * Call this function in `setup()` to initialize the LCD and the custom
     * characters used as up and down arrows
     *
     * @param lcd_Addr address of the LCD on the I2C bus (default 0x27)
     * @param menu menu to display
     */
    void setupLcdWithMenu(uint8_t lcd_Addr, MenuItem* menu);
    /**
     * Call this function to set sub menu items for any main menu item
     *
     * @param position main menu item/where to place the sub menu
     * @param items    sub menu items
     */
    void setSubMenu(uint8_t position, MenuItem* items);
    /**
     * Builder function for a sub menu
     * this functions appends a header and a footer to the final item list
     *
     * @param items array of MenuItems for the sub menu
     * @param size size of items array
     * @return MenuItem list (pointer) with header and footer items included
     */
    MenuItem* buildSubMenu(MenuItem* items, uint8_t size);
    /**
     * Execute an "up press" on menu
     */
    void up();
    /**
     * Execute a "down press" on menu
     */
    void down();
    /**
     * Execute an "enter" action on menu
     */
    void enter();
    /**
     * Execute a "backpress" action on menu
     */
    void back();
    /**
     * Display text at the cursor position
     * used for `Input` type menu items
     *
     * @param text text to display
     */
    void setText(String text);
    /**
     * Get the current cursor position
     *
     * @return `cursorPosition` e.g. 1, 2, 3...
     */
    uint8_t getCursorPosition();
    /**
     * Show a message at the bottom of the screen
     *
     * @param message message to display
     * @param duration how long to display the message
     */
    void displayNotification(char* message, unsigned int duration);
/**
 * Executes any delayed task when appropriate time reaches
 */
void updateTimer();
    /**
     * Get a `MenuItem` at position
     * 
     * @return `MenuItem` - item at `position`
     */
    MenuItem getItemAt(uint8_t position);
};
#endif