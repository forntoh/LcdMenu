#ifndef ItemCommand_H
#define ItemCommand_H

#ifndef MenuConstants_H
#include <Constants.h>
#endif

/**
 * ---
 *
 * # ItemCommand
 *
 * This item type indicates that the current item is a **command**.
 * When `enter()` is invoked, the command *(callback)* bound to this item is
 * invoked.
 */

class ItemCommand : public MenuItem {
   public:
    /**
     * @param key key of the item
     * @param callback reference to callback function
     */
    ItemCommand(const char* key, fptr callback)
        : MenuItem(key, callback, MENU_ITEM_COMMAND) {}
};

#endif