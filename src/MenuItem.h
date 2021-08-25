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

#include <Arduino.h>

typedef void (*fptr)();
typedef void (*fptrInt)(uint8_t);
typedef void (*fptrStr)(String);
//
// menu item types
//
const byte MENU_ITEM_MAIN_MENU_HEADER = 1;
const byte MENU_ITEM_SUB_MENU_HEADER = 2;
const byte MENU_ITEM_SUB_MENU = 3;
const byte MENU_ITEM_COMMAND = 4;
const byte MENU_ITEM_INPUT = 5;
const byte MENU_ITEM_NONE = 6;
const byte MENU_ITEM_TOGGLE = 7;
const byte MENU_ITEM_END_OF_MENU = 8;
const byte MENU_ITEM_LIST = 9;
/**
 * The MenuItem class
 */
class MenuItem {
   private:
    const char* text = NULL;
    char* textOn = NULL;
    char* textOff = NULL;
    fptr callback = NULL;
    fptrInt callbackInt = NULL;
    fptrStr callbackStr = NULL;
    MenuItem* subMenu = NULL;
    byte type = MENU_ITEM_NONE;
    String* items = NULL;

   public:
    /**
     * ## Public Fields
     */

    /**
     * `Boolean` state of the item *(either ON or OFF)*
     */
    boolean isOn = false;
    /**
     * String value of an `ItemInput`
     */
    String value;
    /**
     * Current index of list for `ItemList`
     */
    uint8_t itemIndex = 0;
    /**
     * Number of items in the list for `ItemList`
     */
    uint8_t itemCount = 0;

    MenuItem() = default;
    explicit MenuItem(const char* text) : text(text) {}
    MenuItem(const char* text, fptr callback, byte type)
        : text(text), callback(callback), type(type) {}
    MenuItem(const char* text, fptr callback, MenuItem* subMenu, byte type)
        : text(text), callback(callback), subMenu(subMenu), type(type) {}
    MenuItem(MenuItem* subMenu, byte type) : subMenu(subMenu), type(type) {}
    MenuItem(const char* text, MenuItem* subMenu, byte type)
        : text(text), subMenu(subMenu), type(type) {}
    MenuItem(const char* text, String value, fptrStr callback, byte type)
        : text(text), callbackStr(callback), type(type), value(value) {}
    MenuItem(const char* text, char* textOn, char* textOff, fptrInt callback,
             byte type)
        : text(text),
          textOn(textOn),
          textOff(textOff),
          callbackInt(callback),
          type(type) {}
    MenuItem(const char* text, String* items, uint8_t itemCount, fptrInt callback,
             byte type)
        : text(text),
          callbackInt(callback),
          type(type),
          items(items),
          itemCount(itemCount) {}
    /**
     * ## Getters
     */

    /**
     * Get the text of the item
     * @return `String` - Item's text
     */
    const char* getText() { return text; }
    /**
     * Get the callback of the item
     * @return `ftpr` - Item's callback
     */
    fptr getCallback() { return callback; }
    /**
     * Get the callback of the item
     * @return `fptrInt` - Item's callback
     */
    fptrInt getCallbackInt() { return callbackInt; }
    /**
     * Get the callback of the item
     * @return `fptrStr` - Item's callback
     */
    fptrStr getCallbackStr() { return callbackStr; }
    /**
     * Get the sub menu at item
     * @return `MenuItem*` - Submenu at item
     */
    MenuItem* getSubMenu() { return subMenu; }
    /**
     * Get the type of the item
     * @return `byte` - type of menu item
     */
    byte getType() { return type; }
    /**
     * Get the text when toggle is ON
     * @return `String` - ON text
     */
    char* getTextOn() { return textOn; }
    /**
     * Get the text when toggle is OFF
     * @return `String` - OFF text
     */
    char* getTextOff() { return textOff; }
    /**
     * Get the list of items
     * @return `String*` - List of items
     */
    String* getItems() { return items; }

    /**
     * ## Setters
     */

    /**
     * Set the text of the item
     * @param text text to display for the item
     */
    void setText(const char* text) { this->text = text; }
    /**
     * Set the callback on the item
     * @param callback reference to callback function
     */
    void setCallBack(fptr callback) { this->callback = callback; }
    /**
     * Set the sub menu on the item
     * @param subMenu for the item
     */
    void setSubMenu(MenuItem* subMenu) { this->subMenu = subMenu; }

    /**
     * Operators
     */

    /**
     * Get item at index from the submenu
     * @param index for the item
     */
    MenuItem& operator[](const uint8_t index) {
        return this->getSubMenu()[index];
    }
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
   public:
    /**
     */
    ItemHeader() : MenuItem(NULL, this, MENU_ITEM_MAIN_MENU_HEADER) {}
    /**
     * @param parent the parent menu item
     */
    explicit ItemHeader(MenuItem* parent)
        : MenuItem(NULL, parent, MENU_ITEM_SUB_MENU_HEADER) {}
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
    ItemFooter() : MenuItem(NULL, this, MENU_ITEM_END_OF_MENU) {}
};

/**
 * ---
 *
 * # ItemInput
 *
 * This is an item type where a user can type in information,
 * the information is persisted in the item and can be gotten later by
 * using `item->value`
 */

class ItemInput : public MenuItem {
   public:
    /**
     * @param text text to display for the item
     * @param value the input value
     * @param callback reference to callback function
     */
    ItemInput(const char* text, String value, fptrStr callback)
        : MenuItem(text, value, callback, MENU_ITEM_INPUT) {}
    /**
     */
    ItemInput(char* text, fptrStr callback)
        : MenuItem(text, "", callback, MENU_ITEM_INPUT) {}
};

/**
 * ---
 *
 * # ItemSubMenu
 *
 * This item type indicates that the current item contains a sub menu.
 * The sub menu is opened when `enter()` is invoked.
 */

class ItemSubMenu : public MenuItem {
   public:
    /**
     * @param text text to display for the item
     * @param parent the parent of the sub menu item
     */
    ItemSubMenu(const char* text, MenuItem* parent)
        : MenuItem(text, parent, MENU_ITEM_SUB_MENU) {}
};
/**
 * ---
 *
 * # ItemToggle
 *
 * This item type indicates that the current item is **toggleable**.
 * When `enter()` is invoked, the state of `isOn` is toggled.
 */

class ItemToggle : public MenuItem {
   public:
    /**
     * @param key key of the item
     * @param callback reference to callback function
     */
    ItemToggle(const char* key, fptrInt callback)
        : MenuItem(key, (char*)"ON", (char*)"OFF", callback, MENU_ITEM_TOGGLE) {
    }
    /**
     * @param key key of the item
     * @param textOn display text when ON
     * @param textOff display text when OFF
     * @param callback reference to callback function
     */
    ItemToggle(const char* key, char* textOn, char* textOff, fptrInt callback)
        : MenuItem(key, textOn, textOff, callback, MENU_ITEM_TOGGLE) {}
};

/**
 * ---
 *
 * # ItemCommand
 *
 * This item type indicates that the current item is a **command**.
 * When `enter()` is invoked, the command *(callback)* bound to this item is
 * invoked.
 */

class ItemCommand : public MenuItem {
   public:
    /**
     * @param key key of the item
     * @param callback reference to callback function
     */
    ItemCommand(const char* key, fptr callback)
        : MenuItem(key, callback, MENU_ITEM_COMMAND) {}
};

/**
 * ---
 *
 * # ItemList
 *
 * This item type indicates that the current item is a **list**.
 * - When `left()` is invoked the view cycles down the list
 * - When `right()` is invoked the view cycles up the list, you can use only
 *   `right()` if you have a single button, because once the menu reaches the
 *   end of the list, it automatically starts back from the begining.
 * - When `enter()` is invoked, the command *(callback)* bound to this item is
 *   invoked.
 *
 * > This can be used for other primitive data types, you just need to pass it
 * > as string then parse the result to the desired datatype
 */

class ItemList : public MenuItem {
   public:
    /**
     * @param key key of the items
     * @param items items to display
     * @param itemCount number of items in `items`
     * @param callback reference to callback function
     */
    ItemList(const char* key, String* items, uint8_t itemCount, fptrInt callback)
        : MenuItem(key, items, itemCount, callback, MENU_ITEM_LIST) {}
};

#endif