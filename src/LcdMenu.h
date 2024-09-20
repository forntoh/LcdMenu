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
     * @brief Display Interface.
     */
    DisplayInterface& display;
    /**
     * @brief Currently visible menu screen.
     */
    MenuScreen* screen = NULL;
    /**
     * @brief Enabled flag of menu.
     * Determines whether the screen should be updated after an action. Set it
     * to `false` when you want to display any other content on the screen then
     * set it back to `true` to show the menu.
     */
    bool enabled = true;

  public:
    /**
     * Construct new instance of `LcdMenu`.
     */
    LcdMenu(DisplayInterface& display) : display(display) {}
    /**
     * @brief Get the display.
     * @return the display
     */
    DisplayInterface* getDisplay();
    /**
     * @brief Get the screen that currently on display.
     * @return currently active screen
     */
    MenuScreen* getScreen();
    /**
     * @brief Set new screen to display.
     * The only place that clears whole screen.
     * Then it Will `draw` of new screen screen on display.
     * @param screen the new screen to display
     */
    void setScreen(MenuScreen* screen);
    /**
     * @brief Process the input character.
     * @param c the input character
     * @return `true` if the input was processed successfully
     */
    bool process(const unsigned char c);
    /**
     * @brief Reset current screen to initial state.
     * Moves cursor and view positions to zero.
     */
    void reset();
    /**
     * @brief Hide the menu.
     * When you want to display any other content on the screen then
     * call this function then display your content, later call
     * `show()` to show the menu
     */
    void hide();
    /**
     * @brief Show the menu.
     */
    void show();
    /**
     * @brief Get the current cursor position of current screen.
     * @return the 0-based `cursor` position
     */
    uint8_t getCursor();
    /**
     * @brief Set the current cursor position
     * @param cursor the 0-based `cursor` position
     */
    void setCursor(uint8_t cursor);
    /**
     * @brief Get `MenuItem` at position on current screen.
     * @return `MenuItem` item at `position`
     */
    MenuItem* getItemAt(uint8_t position);
    /**
     * @brief Refresh the current screen.
     */
    void refresh();
};
