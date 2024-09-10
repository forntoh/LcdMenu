/**
 * ---
 *
 * # ItemSubMenu
 *
 * This item type indicates that the current item contains a sub menu.
 * The sub menu is opened when `enter()` is invoked.
 */

#ifndef ItemSubMenu_H
#define ItemSubMenu_H
#include "MenuItem.h"
#include "LcdMenu.h"
#include "MenuScreen.h"

class ItemSubMenu : public MenuItem {
   private:
    LcdMenu* menu = NULL;
    MenuScreen* nextScreen;

   public:
    /**
     * @param text text to display for the item
     * @param parent the parent of the sub menu item
     */
    ItemSubMenu(const char* text, MenuScreen* screen)
        : MenuItem(text), nextScreen(screen) {}

    void initialize(LcdMenu* menu, MenuScreen* screen, DisplayInterface* display) override {
        this->menu = menu;
        this->display = display;
        nextScreen->setParent(screen);
        nextScreen->initialize(menu, display);
    }

    /**
     * Open next screen.
     */
    bool enter() override {
        Serial.println("Changing screen...");
        delay(100);
        menu->setCurrentScreen(nextScreen);
        return true;
    }

    /**
     * Draw text of the item with asterisk (`*`) sign on the end.
     */
    void draw(uint8_t row) override {
        char* buf = new char[strlen(text) + 2];
        concat(text, '*', buf);
        display->drawItem(row, buf);
    }

};

#define ITEM_SUBMENU(...) (new ItemSubMenu(__VA_ARGS__))

#endif
