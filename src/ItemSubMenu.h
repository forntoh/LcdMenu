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

class ItemSubMenu : public ItemHeader {
  public:
    /**
     * @param text text to display for the item
     * @param parent the parent of the sub menu item
     */
    ItemSubMenu(const char* text, MenuItem** parent)
        : ItemHeader(text, parent, MENU_ITEM_SUB_MENU) {}
};

#define ITEM_SUBMENU(...) (new ItemSubMenu(__VA_ARGS__))

#endif
