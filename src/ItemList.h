/**
 * ---
 *
 * # ItemList
 *
 * This item type indicates that the current item is a **list**.
 * - When `left()` is invoked the view cycles down the list
 * - When `right()` is invoked the view cycles up the list, you can use only
 *   `right()` if you have a single button, because once the menu reaches the
 *   end of the list, it automatically starts back from the begining.
 * - When `enter()` is invoked, the command *(callback)* bound to this item is
 *   invoked.
 *
 * > This can be used for other primitive data types, you just need to pass it
 * > as string then parse the result to the desired datatype
 */
#pragma once
#define ItemList_H
#include "MenuItem.h"

class ItemList : public MenuItem {
   public:
    /**
     * @param key key of the items
     * @param items items to display
     * @param itemCount number of items in `items`
     * @param callback reference to callback function
     */
    ItemList(const char* key, String* items, uint8_t itemCount,
             fptrInt callback)
        : MenuItem(key, items, itemCount, callback, MENU_ITEM_LIST) {}
};