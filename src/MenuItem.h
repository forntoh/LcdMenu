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
//
// the MenuItem class
//
class MenuItem {
   private:
    String text;
    String value;
    String textOn;
    String textOff;
    fptr callback = NULL;
    MenuItem* subMenu = NULL;
    byte type = MENU_ITEM_NONE;

   public:
    //
    //
    //
    boolean isOn = false;
    //
    // constructors
    //
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
        : text(text), value(value), callback(callback), type(type) {}
    MenuItem(String text, String textOn, String textOff, fptr callback,
             byte type)
        : text(text),
          textOn(textOn),
          textOff(textOff),
          callback(callback),
          type(type) {}
    //
    // getters
    //
    String getText() { return text; }
    fptr getCallback() { return callback; }
    MenuItem* getSubMenu() { return subMenu; }
    byte getType() { return type; }
    String getValue() { return value; }
    String getTextOn() { return textOn; }
    String getTextOff() { return textOff; }
    //
    // setters
    //
    void setText(String text) { this->text = text; }
    void setCallBack(fptr callback) { this->callback = callback; }
    void setSubMenu(MenuItem* subMenu) { this->subMenu = subMenu; }
};

class ItemHeader : public MenuItem {
   public:
    ItemHeader() : MenuItem(this, MENU_ITEM_MAIN_MENU_HEADER) {}
};

class ItemSubHeader : public MenuItem {
   public:
    ItemSubHeader(MenuItem* parent)
        : MenuItem(parent, MENU_ITEM_SUB_MENU_HEADER) {}
};

class ItemFooter : public MenuItem {
   public:
    ItemFooter() : MenuItem(NULL, MENU_ITEM_END_OF_MENU) {}
};

class ItemInput : public MenuItem {
   public:
    ItemInput(String text, String value, fptr callback)
        : MenuItem(text, value, callback, MENU_ITEM_INPUT) {}
};

class ItemSubMenu : public MenuItem {
   public:
    ItemSubMenu(String text, MenuItem* parent)
        : MenuItem(text, parent, MENU_ITEM_SUB_MENU) {}
};

class ItemToggle : public MenuItem {
   public:
    ItemToggle(String key, fptr callback)
        : MenuItem(key, "", "", callback, MENU_ITEM_TOGGLE) {}
    ItemToggle(String key, String textOn, String textOff, fptr callback)
        : MenuItem(key, textOn, textOff, callback, MENU_ITEM_TOGGLE) {}
};

class ItemCommand : public MenuItem {
   public:
    ItemCommand(String key, fptr callback)
        : MenuItem(key, callback, MENU_ITEM_COMMAND) {}
};

#endif