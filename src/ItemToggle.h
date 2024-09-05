/**
 * ---
 *
 * # ItemToggle
 *
 * This item type indicates that the current item is **toggleable**.
 * When `enter()` is invoked, the state of `isOn` is toggled.
 */

#ifndef ItemToggle_H
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
     * @brief Create an ItemToggle object.
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

    /**
     * @brief Get the integer callback function of this item.
     * @return the integer callback function
     */
    fptrInt getCallbackInt() override { return callback; }

    /**
     * @brief Get the current state of this toggle item.
     * @return the current state
     */
    boolean isOn() override { return enabled; }

    /**
     * @brief Set the current state of this toggle item.
     * @param isOn the new state
     */
    void setIsOn(boolean isOn) override { this->enabled = isOn; }

    const char* getTextOn() override { return this->textOn; }

    const char* getTextOff() override { return this->textOff; }
    
    bool enter(DisplayInterface* lcd) override {
        enabled = !enabled;
        if (callback != NULL) {
            callback(enabled);
        }
        return true;
    };
};

#define ITEM_TOGGLE(...) (new ItemToggle(__VA_ARGS__))

#endif