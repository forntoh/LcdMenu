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
    bool process(Context& context) {
        switch (context.command) {
            case ENTER: return enter(context);
            default: return false;
        }
    }
    /**
     * Open next screen.
     */
    bool enter(Context& context) {
        printLog(F("ItemSubMenu::enter"), text);
        screen->setParent(context.menu->getScreen());
        context.menu->setScreen(screen);
        return true;
    }
};

#define ITEM_SUBMENU(...) (new ItemSubMenu(__VA_ARGS__))

#endif
