#ifndef ITEM_BACK_H
#define ITEM_BACK_H

#include "MenuItem.h"
#include "LcdMenu.h"

class ItemBack : public MenuItem {
   private:
    LcdMenu* menu = NULL;

   public:
    ItemBack(const char* text = "..") : MenuItem(text) {}

    void initialize(LcdMenu* menu, MenuScreen* screen, DisplayInterface* display) override {
        MenuItem::initialize(menu, screen, display);
        this->menu = menu;
    }
    /**
     * Open previous screen.
     */
    bool enter() override {
        Serial.println("Go back...");
        return menu->back();
    }
};

#endif
