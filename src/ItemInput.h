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
    char* getValue() { return value; }

    /**
     * Set the input value for this item.
     *
     * @param value The new input value.
     */
    void setValue(char* value) { this->value = value; }

    /**
     * Get the callback function for this item.
     *
     * @return The function pointer to the callback function.
     */
    fptrStr getCallbackStr() { return callback; }

    void enter(DisplayInterface* display) override {
        if (display->getEditModeEnabled()) {
            return;
        }
        display->setEditModeEnabled(true);
        display->resetBlinker(constrainBlinkerPosition(0, display->getMaxCols()));
    };

    void back(DisplayInterface* display) override {
        // Disable edit mode
        display->setEditModeEnabled(false);
        // Execute callback function
        if (callback != NULL) {
            callback(value);
        }
    };

    void left(DisplayInterface* display) override {
        display->resetBlinker(constrainBlinkerPosition(display->getBlinkerCol() - 1, display->getMaxCols()));
        // Log
        printCmd(F("LEFT"), value[display->getBlinkerCol() - (strlen(text) + 2)]);
    };

    void right(DisplayInterface* display) override {
        display->resetBlinker(constrainBlinkerPosition(display->getBlinkerCol() + 1, display->getMaxCols()));
        // Log
        printCmd(F("RIGHT"), value[display->getBlinkerCol() - (strlen(text) + 2)]);
    };

    uint8_t constrainBlinkerPosition(uint8_t blinkerPosition, uint8_t maxCols) {
        //
        // calculate lower and upper bound
        //
        uint8_t lb = strlen(text) + 2;
        uint8_t ub = lb + strlen(value);
        ub = constrain(ub, lb, maxCols - 2);
        //
        // set cursor position
        //
        return constrain(blinkerPosition, lb, ub);
    }

    void draw(DisplayInterface* display, uint8_t row) override {
        MenuItem::draw(display);
        uint8_t maxCols = display->getMaxCols();
        static char* buf = new char[maxCols];
        substring(value, 0, maxCols - strlen(text) - 2, buf);
        display->drawItem(row, text, ':', buf);
    }

    /**
     * Execute a "backspace cmd" on menu
     *
     * *NB: Works only for `ItemInput` type*
     *
     * Removes the character at the current cursor position.
     */
    void backspace(DisplayInterface* display) override {
        uint8_t p = display->getBlinkerCol() - (strlen(text) + 2) - 1;
        remove(value, p, 1);
        printCmd(F("BACKSPACE"), value);
        display->getBlinkerCol()--;
        update();
    }

    /**
     * Display text at the cursor position
     * used for `Input` type menu items
     * @param character character to append
     */
    void type2(DisplayInterface* display, const char character) override {
        //
        // calculate lower and upper bound
        //
        uint8_t length = strlen(value);
        uint8_t lb = strlen(text) + 2;
        uint8_t ub = lb + length;
        ub = constrain(ub, lb, maxCols - 2);
        printCmd(F("TYPE-CHAR-l"), length);
        printCmd(F("TYPE-CHAR-lb"), lb);
        printCmd(F("TYPE-CHAR-ub"), ub);
        printCmd(F("TYPE-CHAR-bl"), display->getBlinkerCol());
        //
        // update text
        //
        if (display->getBlinkerCol() < ub) {
            char start[10];
            char end[10];
            char* joined = new char[maxCols - lb];
            substring(value, 0, display->getBlinkerCol() - lb, start);
            substring(value, display->getBlinkerCol() + 1 - lb, length, end);
            concat(start, character, end, joined);
            value = joined;
        } else {
            char* buf = new char[length + 2];
            concat(value, character, buf);
            value = buf;
        }
        //
        // update blinker position
        //
        display->getBlinkerCol()++;
        // Log
        printCmd(F("TYPE-CHAR"), character);
    }

    void drawChar(DisplayInterface* display, char c) override { isCharPickerActive = display->drawChar(c); }

    /**
     * Clear the value of the input field
     */
    void clear(DisplayInterface* display) override {
        //
        // set the value
        //
        value = (char*)"";
        // Log
        printCmd(F("CLEAR"), value);
        //
        // update blinker position
        //
        display->getBlinkerCol() = 0;
    }

};

#define ITEM_INPUT(...) (new ItemInput(__VA_ARGS__))

#endif  // ITEM_INPUT_H
