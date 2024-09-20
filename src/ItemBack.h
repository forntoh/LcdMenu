/**
 * ---
 *
 * # ItemBack
 *
 * This item type indicates that the current item is a **command**.
 * When `enter()` is invoked, the command *(callback)* bound to this item is
 * invoked.
 */

#ifndef ItemBack_H
#define ItemBack_H

// Include the header file for the base class.
#include "LcdMenu.h"
#include "MenuItem.h"

// Declare a class for menu items that represent commands.
class ItemBack : public MenuItem {
  public:
    /**
     * Construct a new ItemBack object.
     * @param text The text of the item.
     */
    ItemBack(const char* text = "..") : MenuItem(text) {}

  protected:
    bool process(LcdMenu* menu, const unsigned char command) override {
        switch (command) {
            case ENTER:
                changeScreen(menu);
                return true;
            default:
                return false;
        }
    }
    void changeScreen(LcdMenu* menu) {
        menu->process(BACK);
    }
};

#define ITEM_BACK(...) (new ItemBack(__VA_ARGS__))

#endif
