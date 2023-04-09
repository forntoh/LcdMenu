/**
 * ---
 *
 * # ItemSubMenu
 *
 * This item type indicates that the current item contains a sub menu.
 * The sub menu is opened when `enter()` is invoked.
 */

#pragma once
#define ItemSubMenu_H
#include "MenuItem.h"

class ItemSubMenu : public MenuItem {
   public:
    /**
     * @param text text to display for the item
     * @param parent the parent of the sub menu item
     */
    ItemSubMenu(const char* text, MenuItem* parent)
        : MenuItem(text, parent, MENU_ITEM_SUB_MENU) {}
};