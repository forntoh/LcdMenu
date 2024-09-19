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

#include "MenuScreen.h"
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
    /**
     * Display Interface
     */
    DisplayInterface& display;
    /**
     * Currently visible menu screen.
     */
    MenuScreen* currentScreen = NULL;
    /**
     * Determines whether the screen should be updated after an action. Set it
     * to `false` when you want to display any other content on the screen then
     * set it back to `true` to show the menu.
     */
    bool enableUpdate = true;

  public:
    /**
     * # Constructor
     */
    LcdMenu(DisplayInterface& display) : display(display) {}
    DisplayInterface* getDisplay() {
        return &display;
    }
    MenuScreen* getCurrentScreen() {
        return currentScreen;
    }
    void setCurrentScreen(MenuScreen* screen);
    void initialize(MenuScreen* screen);
    bool process(const unsigned char c);
    /**
     * Reset the display
     */
    void resetMenu();
    /**
     * When you want to display any other content on the screen then
     * call this function then display your content, later call
     * `show()` to show the menu
     */
    void hide();
    /**
     * Show the menu
     */
    void show();
    /**
     * Get the current cursor position
     * @return `cursorPosition` e.g. 1, 2, 3...
     */
    uint8_t getCursorPosition();
    /**
     * Set the current cursor position
     * @param position
     */
    void setCursorPosition(uint8_t position);
    /**
     * Get a `MenuItem` at position
     * @return `MenuItem` - item at `position`
     */
    MenuItem* getItemAt(uint8_t position);
    void refresh();
};
