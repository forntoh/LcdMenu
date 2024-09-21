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
#include "LcdMenu.h"
#include "MenuItem.h"

class ItemSubMenu : public MenuItem {
  private:
    MenuScreen*& screen;

  public:
    /**
     * @param text text to display for the item
     * @param screen the next screen to show
     */
    ItemSubMenu(const char* text, MenuScreen*& screen) : MenuItem(text), screen(screen) {}

  protected:
    bool process(LcdMenu* menu, const unsigned char command) {
        switch (command) {
            case ENTER:
                changeScreen(menu);
                return true;
            default:
                return false;
        }
    }
    void changeScreen(LcdMenu* menu) {
        printLog(F("ItemSubMenu::changeScreen"), text);
        screen->setParent(menu->getScreen());
        menu->setScreen(screen);
    }
};

#define ITEM_SUBMENU(...) (new ItemSubMenu(__VA_ARGS__))

#endif
