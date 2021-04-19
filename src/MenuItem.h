/*
  MenuItem.h - MenuItem declaration and definition

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

#ifndef MenuItem_H
#define MenuItem_H

typedef void (*fptr)();
//
// menu item types
//
const byte MENU_ITEM_MAIN_MENU_HEADER = 0;
const byte MENU_ITEM_SUB_MENU_HEADER = 1;
const byte MENU_ITEM_SUB_MENU = 2;
const byte MENU_ITEM_COMMAND = 3;
const byte MENU_ITEM_INPUT = 4;
const byte MENU_ITEM_NONE = 5;
const byte MENU_ITEM_TOGGLE = 6;
const byte MENU_ITEM_END_OF_MENU = 7;
/**
 * The MenuItem class
 */
class MenuItem {
   private:
    String text;
    String textOn;
    String textOff;
    fptr callback = NULL;
    MenuItem* subMenu = NULL;
    byte type = MENU_ITEM_NONE;

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

    MenuItem() = default;
    MenuItem(String text) : text(text) {}
    MenuItem(String text, fptr callback, byte type)
        : text(text), callback(callback), type(type) {}
    MenuItem(String text, fptr callback, MenuItem* subMenu, byte type)
        : text(text), callback(callback), subMenu(subMenu), type(type) {}
    MenuItem(MenuItem* subMenu, byte type) : subMenu(subMenu), type(type) {}
    MenuItem(String text, MenuItem* subMenu, byte type)
        : text(text), subMenu(subMenu), type(type) {}
    MenuItem(String text, String value, fptr callback, byte type)
        : text(text), callback(callback), type(type), value(value) {}
    MenuItem(String text, String textOn, String textOff, fptr callback,
             byte type)
        : text(text),
          textOn(textOn),
          textOff(textOff),
          callback(callback),
          type(type) {}
    /**
     * ## Getters
     */

    /**
     * Get the text of the item
     * @return `String` - Item's text
     */
    String getText() { return text; }
    /**
     * Get the callback of the item
     * @return `ftpr` - Item's callback
     */
    fptr getCallback() { return callback; }
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
    String getTextOn() { return textOn; }
    /**
     * Get the text when toggle is OFF
     * @return `String` - OFF text
     */
    String getTextOff() { return textOff; }

    /**
     * ## Setters
     */

    /**
     * Set the text of the item
     * @param text text to display for the item
     */
    void setText(String text) { this->text = text; }
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
};

/**
 * <br>
 * 
 * # ItemHeader
 */

/**
 */
class ItemHeader : public MenuItem {
   public:
    /**
     * ### Constructor
     */

    /**
     */
    ItemHeader()
        : MenuItem(this, MENU_ITEM_MAIN_MENU_HEADER) {}
};

/**
 * # ItemSubHeader
 */

/**
 */
class ItemSubHeader : public MenuItem {
   public:
    /**
     * ### Constructor
     */

    /**
     * @param parent the parent menu item
     */
    ItemSubHeader(MenuItem* parent)
        : MenuItem(parent, MENU_ITEM_SUB_MENU_HEADER) {}
};

/**
 * # ItemFooter
 */

/**
 */
class ItemFooter : public MenuItem {
   public:
    /**
     * ### Constructor
     */

    /**
     */
    ItemFooter()
        : MenuItem(NULL, MENU_ITEM_END_OF_MENU) {}
};

/** 
 * # ItemInput
 */

/**
 */
class ItemInput : public MenuItem {
   public:
    /**
     * ### Constructor
     */

    /**
     * @param text text to display for the item
     * @param value the input value
     * @param callback reference to callback function
     */
    ItemInput(String text, String value, fptr callback)
        : MenuItem(text, value, callback, MENU_ITEM_INPUT) {}
};

/**
 * # ItemSubMenu
 */

/**
 */
class ItemSubMenu : public MenuItem {
   public:
    /**
     * ### Constructor
     */

    /**
     * @param text text to display for the item
     * @param parent the parent of the sub menu item
     */
    ItemSubMenu(String text, MenuItem* parent)
        : MenuItem(text, parent, MENU_ITEM_SUB_MENU) {}
};

/**
 * # ItemToggle
 */

/**
 */
class ItemToggle : public MenuItem {
   public:
    /**
     * ### Constructor
     */

    /**
     * @param key key of the item
     * @param callback reference to callback function
     */
    ItemToggle(String key, fptr callback)
        : MenuItem(key, "", "", callback, MENU_ITEM_TOGGLE) {}
    /**
     * @param key key of the item
     * @param textOn display text when ON
     * @param textOff display text when OFF
     * @param callback reference to callback function
     */
    ItemToggle(String key, String textOn, String textOff, fptr callback)
        : MenuItem(key, textOn, textOff, callback, MENU_ITEM_TOGGLE) {}
};

/**
 * # ItemCommand
 */

/**
 */
class ItemCommand : public MenuItem {
   public:
    /**
     * ### Constructor
     */

    /**
     * @param key key of the item
     * @param callback reference to callback function
     */
    ItemCommand(String key, fptr callback)
        : MenuItem(key, callback, MENU_ITEM_COMMAND) {}
};

#endif