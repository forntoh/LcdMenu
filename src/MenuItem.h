/*
  MenuItem.h - MenuItem declaration and definition

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

#ifndef MenuItem_H
#define MenuItem_H

#include "Constants.h"

/**
 * The MenuItem class
 */
class MenuItem {
   protected:
    const char* text = NULL;
    byte type = MENU_ITEM_NONE;

   public:
    MenuItem() = default;
    MenuItem(const char* text) : text(text) {}
    MenuItem(const char* text, byte type) : text(text), type(type) {}
    /**
     * ## Getters
     */

    /**
     * `Boolean` state of the item *(either ON or OFF)*
     */
    virtual boolean isOn() { return false; }
    /**
     * String value of an `ItemInput`
     */
    virtual String getValue() { return ""; }
    /**
     * Get the text of the item
     * @return `String` - Item's text
     */
    virtual const char* getText() { return text; }
    /**
     * Get the callback of the item
     * @return `ftpr` - Item's callback
     */
    virtual fptr getCallback() { return NULL; }
    /**
     * Get the callback of the item
     * @return `fptrInt` - Item's callback
     */
    virtual fptrInt getCallbackInt() { return NULL; }
    /**
     * Get the callback of the item
     * @return `fptrStr` - Item's callback
     */
    virtual fptrStr getCallbackStr() { return NULL; }
    /**
     * Get the sub menu at item
     * @return `MenuItem*` - Submenu at item
     */
    virtual MenuItem* getSubMenu() { return NULL; }
    /**
     * Get the type of the item
     * @return `byte` - type of menu item
     */
    byte getType() { return type; }
    /**
     * Get the text when toggle is ON
     * @return `String` - ON text
     */
    virtual const char* getTextOn() { return ""; }
    /**
     * Get the text when toggle is OFF
     * @return `String` - OFF text
     */
    virtual const char* getTextOff() { return ""; }
    /**
     * Current index of list for `ItemList`
     */
    virtual uint8_t getItemIndex() { return 0; }
    /**
     * Number of items in the list for `ItemList`
     */
    virtual uint8_t getItemCount() { return 0; };
    /**
     * Get the list of items
     * @return `String*` - List of items
     */
    virtual String* getItems() { return NULL; }

    /**
     * ## Setters
     */

    /**
     * `Boolean` state of the item *(either ON or OFF)*
     */
    virtual void setIsOn(boolean isOn){};
    /**
     * String value of an `ItemInput`
     */
    virtual void setValue(String value){};
    /**
     * Set the text of the item
     * @param text text to display for the item
     */
    void setText(const char* text){};
    /**
     * Set the callback on the item
     * @param callback reference to callback function
     */
    virtual void setCallBack(fptr callback){};
    /**
     * Current index of list for `ItemList`
     */
    virtual void setItemIndex(uint8_t itemIndex){};

    /**
     * Operators
     */

    /**
     * Get item at index from the submenu
     * @param index for the item
     */
    MenuItem& operator[](const uint8_t index);
};

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
    MenuItem* parent = NULL;

    ItemHeader(const char* text, MenuItem* parent, byte type)
        : MenuItem(NULL, type), parent(parent) {}

   public:
    /**
     */
    ItemHeader() : ItemHeader(this) {}
    /**
     * @param parent the parent menu item
     */
    ItemHeader(MenuItem* parent)
        : ItemHeader("", parent, MENU_ITEM_SUB_MENU_HEADER) {}

    MenuItem* getSubMenu() override { return this->parent; };
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

#endif