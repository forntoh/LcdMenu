#ifndef MenuItem_H
#define MenuItem_H

class MenuItem {
   private:
    char* text;
    void (*callback)() = NULL;
    MenuItem* subMenu = NULL;

   public:
    MenuItem(char* text) : text(text) {}
    MenuItem(char* text, void (*callback)()) : text(text), callback(callback) {}
    MenuItem(char* text, void (*callback)(), MenuItem* subMenu)
        : text(text), callback(callback), subMenu(subMenu) {}
};

#endif