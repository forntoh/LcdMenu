#ifndef MenuItem_H
#define MenuItem_H
//
// menu item types
//
const byte MENU_ITEM_TYPE_MAIN_MENU_HEADER = 0;
const byte MENU_ITEM_TYPE_SUB_MENU_HEADER = 1;
const byte MENU_ITEM_TYPE_SUB_MENU = 2;
const byte MENU_ITEM_TYPE_COMMAND = 3;
const byte MENU_ITEM_TYPE_INPUT = 4;
const byte MENU_ITEM_TYPE_NONE = 5;
const byte MENU_ITEM_TYPE_END_OF_MENU = 6;
//
// the MenuItem class
//
class MenuItem {
   private:
    char* text;
    void (*callback)() = NULL;
    MenuItem* subMenu = NULL;

   public:
    //
    // constructors
    //
    MenuItem(char* text) : text(text) {}
    MenuItem(char* text, void (*callback)()) : text(text), callback(callback) {}
    MenuItem(char* text, void (*callback)(), MenuItem* subMenu)
        : text(text), callback(callback), subMenu(subMenu) {}
    //
    // getters
    //
    char* getText() { return text; }
    void* getCallback() { return callback; }
    MenuItem* getSubMenu() { return subMenu; }
    //
    // setters
    //
    void setText(char* text) { this->text = text; }
    void setCallBack(void (*callback)()) { this->callback = callback; }
    void setSubMenu(MenuItem* subMenu) { this->subMenu = subMenu; }
};

#endif