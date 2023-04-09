/**
 * ---
 *
 * # ItemToggle
 *
 * This item type indicates that the current item is **toggleable**.
 * When `enter()` is invoked, the state of `isOn` is toggled.
 */

#pragma once
#define ItemToggle_H
#include "MenuItem.h"

class ItemToggle : public MenuItem {
   private:
    bool enabled = false;
    const char* textOn = NULL;
    const char* textOff = NULL;
    fptrInt callback = NULL;

   public:
    /**
     * @param key key of the item
     * @param callback reference to callback function
     */
    ItemToggle(const char* key, fptrInt callback)
        : ItemToggle(key, "ON", "OFF", callback) {}
    /**
     * @param key key of the item
     * @param textOn display text when ON
     * @param textOff display text when OFF
     * @param callback reference to callback function
     */
    ItemToggle(const char* key, const char* textOn, const char* textOff,
               fptrInt callback)
        : MenuItem(key, MENU_ITEM_TOGGLE),
          textOn(textOn),
          textOff(textOff),
          callback(callback) {}

    fptrInt getCallbackInt() override { return callback; }

    boolean isOn() override { return enabled; }

    void setIsOn(boolean isOn) override { this->enabled = isOn; }
};