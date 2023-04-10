/**
 * ---
 *
 * # ItemInput
 *
 * This is an item type where a user can type in information,
 * the information is persisted in the item and can be gotten later by
 * using `item->value`
 */

#ifndef ItemInput_H
#define ItemInput_H

// Include the header file for the base class.
#include "MenuItem.h"

// Declare a class for menu items that allow the user to input information.
class ItemInput : public MenuItem {
   private:
    // Declare a string to hold the input value.
    String value;

    // Declare a function pointer for the input callback.
    fptrStr callback;

   public:
    /**
     * Construct a new ItemInput object with an initial value.
     *
     * @param text The text to display for the item.
     * @param value The initial value for the input.
     * @param callback A reference to the callback function to be invoked when
     * the input is submitted.
     */
    ItemInput(const char* text, String value, fptrStr callback)
        : MenuItem(text, MENU_ITEM_INPUT), value(value), callback(callback) {}

    /**
     * Construct a new ItemInput object with no initial value.
     *
     * @param text The text to display for the item.
     * @param callback A reference to the callback function to be invoked when
     * the input is submitted.
     */
    ItemInput(const char* text, fptrStr callback)
        : ItemInput(text, "", callback) {}

    /**
     * Get the current input value for this item.
     *
     * @return The current input value as a string.
     */
    String getValue() override { return value; }

    /**
     * Set the input value for this item.
     *
     * @param value The new input value.
     */
    void setValue(String value) override { this->value = value; }

    /**
     * Get the callback function for this item.
     *
     * @return The function pointer to the callback function.
     */
    fptrStr getCallbackStr() override { return callback; }
};

#define ITEM_INPUT(...) (new ItemInput(__VA_ARGS__))

#endif  // ITEM_INPUT_H
