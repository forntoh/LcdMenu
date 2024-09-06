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
#include <utils/utils.h>

// Declare a class for menu items that allow the user to input information.
class ItemInput : public MenuItem {
   private:
    // Declare a string to hold the input value.
    char* value;

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
    ItemInput(const char* text, char* value, fptrStr callback)
        : MenuItem(text, MENU_ITEM_INPUT), value(value), callback(callback) {}

    /**
     * Construct a new ItemInput object with no initial value.
     *
     * @param text The text to display for the item.
     * @param callback A reference to the callback function to be invoked when
     * the input is submitted.
     */
    ItemInput(const char* text, fptrStr callback)
        : ItemInput(text, (char*)"", callback) {}

    /**
     * Get the current input value for this item.
     *
     * @return The current input value as a string.
     */
    char* getValue() override { return value; }

    /**
     * Set the input value for this item.
     *
     * @param value The new input value.
     */
    void setValue(char* value) override { this->value = value; }

    /**
     * Get the callback function for this item.
     *
     * @return The function pointer to the callback function.
     */
    fptrStr getCallbackStr() override { return callback; }

    bool enter(DisplayInterface* lcd) override {
        lcd->setEditModeEnabled(true);
        return false;
    };

    bool back(DisplayInterface* lcd) override {
        // Disable edit mode
        lcd->setEditModeEnabled(false);
        // Execute callback function
        if (callback != NULL) {
            callback(value);
        }
        return true;
    };

    bool left(DisplayInterface* lcd) override {
        lcd->blinkerPosition--;
        lcd->resetBlinker();
        // Log
        printCmd(F("LEFT"), value[lcd->blinkerPosition - (strlen(this->getText()) + 2)]);
        return false;
    };

    bool right(DisplayInterface* lcd) override {
        lcd->blinkerPosition++;
        lcd->resetBlinker();
        // Log
        printCmd(F("RIGHT"), value[lcd->blinkerPosition - (strlen(this->getText()) + 2)]);
        return false;
    };

    void draw(DisplayInterface* lcd) override {
        uint8_t maxCols = lcd->getMaxCols();
        static char* buf = new char[maxCols];
        substring(value, 0, maxCols - strlen(text) - 2, buf);
        lcd->getPrint()->print(":");
        lcd->getPrint()->print(buf);
    }

};

#define ITEM_INPUT(...) (new ItemInput(__VA_ARGS__))

#endif  // ITEM_INPUT_H
