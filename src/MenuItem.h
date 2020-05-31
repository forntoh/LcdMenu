#ifndef MenuItem_H
#define MenuItem_H

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