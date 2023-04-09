/**
 * ---
 *
 * # ItemInput
 *
 * This is an item type where a user can type in information,
 * the information is persisted in the item and can be gotten later by
 * using `item->value`
 */

#pragma once
#define ItemInput_H
#include "MenuItem.h"

class ItemInput : public MenuItem {
   private:
    String value;
    fptrStr callback;

   public:
    /**
     * @param text text to display for the item
     * @param value the input value
     * @param callback reference to callback function
     */
    ItemInput(const char* text, String value, fptrStr callback)
        : MenuItem(text, MENU_ITEM_INPUT), value(value), callback(callback) {}
    /**
     */
    ItemInput(const char* text, fptrStr callback)
        : ItemInput(text, "", callback) {}

    String getValue() override { return value; }

    void setValue(String value) override { this->value = value; }

    fptrStr getCallbackStr() override { return callback; }
};