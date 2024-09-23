#ifndef ItemBack_H
#define ItemBack_H

#include "LcdMenu.h"
#include "MenuItem.h"

/**
 * @class ItemBack
 * @brief A menu item that navigates back to the previous screen.
 * 
 * The ItemBack class represents a menu item that, when selected, 
 * navigates back to the previous screen in the menu system.
 * 
 * @details
 * This class inherits from the MenuItem class and overrides the 
 * process method to handle the ENTER command by changing the screen.
 * 
 * @note The default text for this item is "..".
 */
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
