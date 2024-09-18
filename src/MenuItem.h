/*
  MenuItem.h - MenuItem declaration and definition

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

#ifndef MenuItem_H
#define MenuItem_H

#include "display/DisplayInterface.h"
#include "utils/constants.h"
#include <utils/utils.h>

class LcdMenu;

/**
 * @brief The MenuItem class.
 *
 * ┌────────────────────────────┐
 * │ > T E X T                  │
 * └────────────────────────────┘
 *
 */
class MenuItem {
    friend LcdMenu;

  protected:
    const char* text = NULL;

  public:
    MenuItem(const char* text) : text(text) {}
    /**
     * Get the text of the item
     * @return `String` - Item's text
     */
    virtual const char* getText() { return text; }
    /**
     * Get the sub menu at item
     * @return `MenuItem*` - Submenu at item
     */
    virtual MenuItem** getSubMenu() { return NULL; }
    /**
     * Set the text of the item
     * @param text text to display for the item
     */
    void setText(const char* text) { this->text = text; };

  protected:
    struct Context {
        LcdMenu* menu;
        DisplayInterface* display;
        const unsigned char command;
    };
    /**
     * @brief Process a command decoded in 1 byte.
     * It can be a printable character or a control command like `ENTER` or `LEFT`.
     * Return value is used to determine operation was successful or ignored.
     * If the parent of item received that handle was ignored it can execute it's own action on this command.
     * Thus, the item always has priority in processing; if it is ignored, it is delegated to the parent element.
     * Behaviour is very similar to Even Bubbling in JavaScript.
     * @param context the context of call, contains at least character command (can be a printable character or a control command) and backreference to menu
     * @return true if command was successfully handled by item.
     */
    virtual bool process(Context context) { return false; };
    /**
     * @brief Draw this menu item on specified display on current line.
     * @param display the display that should be used for draw
     */
    virtual void draw(DisplayInterface* display) {
        draw(display, display->getCursorRow());
    };
    /**
     * @brief Draw this menu item on specified display on specified line.
     * @param display the display that should be used for draw
     * @param row the number of row to draw on
     */
    virtual void draw(DisplayInterface* display, uint8_t row) {
        uint8_t maxCols = display->getMaxCols();
        static char* buf = new char[maxCols];
        substring(text, 0, maxCols - 2, buf);
        display->drawItem(row, buf);
    };

  public:
    /**
     * Get item at index from the submenu
     * @param index for the item
     */
    virtual MenuItem* operator[](const uint8_t index) { return NULL; };
};
#define ITEM_BASIC(...) (new MenuItem(__VA_ARGS__))

class ItemHeader : public MenuItem {
  protected:
    MenuItem** parent = NULL;

    ItemHeader(const char* text, MenuItem** parent)
        : MenuItem(text), parent(parent) {}

  public:
    /**
     */
    ItemHeader() : ItemHeader("", NULL) {}
    /**
     * @param parent the parent menu item
     */
    ItemHeader(MenuItem** parent)
        : ItemHeader("", parent) {}

    MenuItem** getSubMenu() override { return this->parent; };

    MenuItem* operator[](const uint8_t index) override {
        return getSubMenu()[index];
    }
};

class ItemFooter : public MenuItem {
  public:
    /**
     */
    ItemFooter() : MenuItem(NULL) {}
};

#define MAIN_MENU(...)           \
    extern MenuItem* mainMenu[]; \
    MenuItem* mainMenu[] = {new ItemHeader(), __VA_ARGS__, nullptr}

#define SUB_MENU(subMenu, parent, ...) \
    MenuItem* subMenu[] = {new ItemHeader(parent), __VA_ARGS__, nullptr}

#endif
