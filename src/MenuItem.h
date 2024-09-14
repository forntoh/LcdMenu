/*
  MenuItem.h - MenuItem declaration and definition

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

#ifndef MenuItem_H
#define MenuItem_H

#include "interface/DisplayInterface.h"
#include "utils/constants.h"
#include <utils/utils.h>

/**
 * The MenuItem class
 */
class MenuItem {
   protected:
    const char* text = NULL;
    byte type = MENU_ITEM_NONE;
    DisplayInterface* display;

   public:
    MenuItem(const char* text) : text(text) {}
    MenuItem(const char* text, byte type) : text(text), type(type) {}
    virtual void initialize(DisplayInterface* display) {
        this->display = display;
        MenuItem** subMenu = this->getSubMenu();
        if (subMenu == NULL) {
            return;
        }
        for (uint8_t i = 1; subMenu[i]->getType() != MENU_ITEM_END_OF_MENU; ++i) {
            subMenu[i]->initialize(display);
        }
    }

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
     * Get the type of the item
     * @return `byte` - type of menu item
     */
    byte getType() { return type; }
    /**
     * Set the text of the item
     * @param text text to display for the item
     */
    void setText(const char* text) { this->text = text; };

    virtual void up() {};
    virtual void down() {};
    virtual void enter() {};
    virtual void back() {};
    virtual void left() {};
    virtual void right() {};
    virtual void backspace() {};
    virtual void typeChar(const char character) {};
    virtual void clear() {};
    virtual void draw() {
        draw(display->getCursorRow());
    };
    virtual void draw(uint8_t row) {
        uint8_t maxCols = display->getMaxCols();
        static char* buf = new char[maxCols];
        substring(text, 0, maxCols - 2, buf);
        display->drawItem(row, buf);
    };

    /**
     * Operators
     */

    /**
     * Get item at index from the submenu
     * @param index for the item
     */
    virtual MenuItem* operator[](const uint8_t index) { return NULL; };
};
#define ITEM_BASIC(...) (new MenuItem(__VA_ARGS__))

/**
 *
 * # ItemHeader
 *
 * This item must be present at the all menus collections *(the first item in
 * the array)*.
 *
 * **Example**
 *
 * ```cpp
 * MenuItem mainMenu[] = {ItemHeader(),
 *                        MenuItem("Item 1"),
 *                        MenuItem("Item 2"),
 *                        ...
 * ```
 */

class ItemHeader : public MenuItem {
   protected:
    MenuItem** parent = NULL;

    ItemHeader(const char* text, MenuItem** parent, byte type)
        : MenuItem(text, type), parent(parent) {}

   public:
    /**
     */
    ItemHeader() : ItemHeader("", NULL, MENU_ITEM_MAIN_MENU_HEADER) {}
    /**
     * @param parent the parent menu item
     */
    ItemHeader(MenuItem** parent)
        : ItemHeader("", parent, MENU_ITEM_SUB_MENU_HEADER) {}

    MenuItem** getSubMenu() override { return this->parent; };

    MenuItem* operator[](const uint8_t index) override {
        return getSubMenu()[index];
    }
};

/**
 * ---
 *
 * # ItemFooter
 *
 * This item must be present at the all menus *(the last item in the array)*
 *
 * **Example**
 *
 * ```cpp
 * MenuItem mainMenu[] = {ItemHeader(),
 *                        MenuItem("Item 1"),
 *                        MenuItem("Item 2"),
 *                        ...
 *                        ItemFooter()};
 * ```
 */

class ItemFooter : public MenuItem {
   public:
    /**
     */
    ItemFooter() : MenuItem(NULL, MENU_ITEM_END_OF_MENU) {}
};

#define MAIN_MENU(...)           \
    extern MenuItem* mainMenu[]; \
    MenuItem* mainMenu[] = {new ItemHeader(), __VA_ARGS__, new ItemFooter()}

#define SUB_MENU(subMenu, parent, ...)                          \
    MenuItem* subMenu[] = {new ItemHeader(parent), __VA_ARGS__, \
                           new ItemFooter()}

#endif