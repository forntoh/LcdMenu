/**
 * ---
 *
 * # ItemCommand
 *
 * This item type indicates that the current item is a **command**.
 * When `enter()` is invoked, the command *(callback)* bound to this item is
 * invoked.
 */

#pragma once
#define ItemCommand_H
#include "MenuItem.h"

class ItemCommand : public MenuItem {
   private:
    fptr callback;

   public:
    /**
     * @param key key of the item
     * @param callback reference to callback function
     */
    ItemCommand(const char* key, fptr callback)
        : MenuItem(key, MENU_ITEM_COMMAND) {
        this->callback = callback;
    }

    fptr getCallback() override { return callback; }

    void setCallBack(fptr callback) override { this->callback = callback; };
};