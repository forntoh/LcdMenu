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
const byte MENU_ITEM_END_OF_MENU = 6;
//
// the MenuItem class
//
class MenuItem {
   private:
    char* text = "";
    fptr callback = NULL;
    MenuItem* subMenu = NULL;
    byte type = MENU_ITEM_NONE;

   public:
    //
    // constructors
    //
    MenuItem(char* text) : text(text) {}
    MenuItem(char* text, fptr callback, byte type)
        : text(text), callback(callback), type(type) {}
    MenuItem(char* text, fptr callback, MenuItem* subMenu, byte type)
        : text(text), callback(callback), subMenu(subMenu), type(type) {}
    MenuItem(MenuItem* subMenu, byte type)
        : callback(callback), subMenu(subMenu), type(type) {}
    MenuItem(char* text, MenuItem* subMenu, byte type)
        : text(text), callback(callback), subMenu(subMenu), type(type) {}
    //
    // getters
    //
    char* getText() { return text; }
    fptr getCallback() { return callback; }
    MenuItem* getSubMenu() { return subMenu; }
    byte getType() { return type; }
    //
    // setters
    //
    void setText(char* text) { this->text = text; }
    void setCallBack(fptr callback) { this->callback = callback; }
    void setSubMenu(MenuItem* subMenu) { this->subMenu = subMenu; }
};

class ItemHeader : public MenuItem {
   public:
    ItemHeader() : MenuItem(this, MENU_ITEM_MAIN_MENU_HEADER) {}
};

class ItemSubHeader : public MenuItem {
   public:
    ItemSubHeader(MenuItem* parent) : MenuItem(parent, MENU_ITEM_SUB_MENU_HEADER) {}
};

class ItemFooter : public MenuItem {
   public:
    ItemFooter() : MenuItem(NULL, MENU_ITEM_END_OF_MENU) {}
};

class ItemInput : public MenuItem {
   public:
    ItemInput(char* text, fptr callback) : MenuItem(text, callback, MENU_ITEM_INPUT) {}
};

class ItemSubMenu : public MenuItem {
   public:
    ItemSubMenu(char* text, MenuItem* parent) : MenuItem(text, parent, MENU_ITEM_SUB_MENU) {}
};

#endif