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
#include "ActionHandler.h"
#include "MenuScreen.h"

/**
 * The LcdMenu class contains all fields and methods to manipulate the menu
 * items.
 */
class LcdMenu: public ActionHandler {
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

    /**
     * Will prevent left and right movement when in edit mode and character
     * picker is active
     */
    bool isCharPickerActive = false;

   public:

    /**
     * # Constructor
     */
    LcdMenu(DisplayInterface& display) : display(display) {}

    DisplayInterface* getDisplay(){
        return &display;
    }

    MenuScreen* getCurrentScreen() {
        return currentScreen;
    }

    void setCurrentScreen(MenuScreen* screen) {
        currentScreen = screen;
        display.clear();
        currentScreen->draw();
    }

    void initialize(MenuScreen* screen) {
        display.begin();
        currentScreen = screen;
        currentScreen->initialize(this, &display);
        currentScreen->draw();
    }

    /**
     * Reset the display
     */
    void resetMenu() { this->currentScreen->reset(); }

    bool up() override { return enableUpdate ? currentScreen->up() : false; }
    bool down() override { return enableUpdate ? currentScreen->down() : false; }
    bool enter() override { return enableUpdate ? currentScreen->enter() : false; }
    bool back() override { return enableUpdate ? currentScreen->back() : false; }
    bool left() override { return enableUpdate ? currentScreen->left() : false; }
    bool right() override { return enableUpdate ? currentScreen->right() : false; }
    bool backspace() override { return enableUpdate ? currentScreen->backspace() : false; }
    bool type(const char c) override { return enableUpdate ? currentScreen->type(c) : false; }
    bool clear() override  { return enableUpdate ? currentScreen->clear() : false; }

    void drawChar(char c) {
        if (enableUpdate && display.getEditModeEnabled()) {
            return;
        }
        isCharPickerActive = display.drawChar(c); 
    }

    /**
     * When you want to display any other content on the screen then
     * call this function then display your content, later call
     * `show()` to show the menu
     */
    void hide() {
        enableUpdate = false;
        display.clear();
    }
    /**
     * Show the menu
     */
    void show() {
        enableUpdate = true;
        display.clear();
        currentScreen->draw();
    }
    /**
     * Get the current cursor position
     * @return `cursorPosition` e.g. 1, 2, 3...
     */
    uint8_t getCursorPosition() { return this->currentScreen->getCursor(); }
    /**
     * Set the current cursor position
     * @param position
     */
    void setCursorPosition(uint8_t position) {
        this->currentScreen->setCursor(position);
    }
    /**
     * Check if currently displayed menu is a sub menu.
     */
    bool isSubMenu() {
        // TODO
        return false;
    }
    /**
     * Get a `MenuItem` at position
     * @return `MenuItem` - item at `position`
     */
    MenuItem* getItemAt(uint8_t position) { return currentScreen->getItemAt(position); }
    /**
     * Get a `MenuItem` at position using operator function
     * e.g `menu[menu.getCursorPosition()]` will return the item at the
     * current cursor position NB: This is relative positioning (i.e. if a
     * submenu is currently being displayed, menu[1] will return item 1 in
     * the current menu)
     * @return `MenuItem` - item at `position`
     */
    MenuItem* operator[](const uint8_t position) {
        return currentScreen->getItemAt(position);
    }
};
