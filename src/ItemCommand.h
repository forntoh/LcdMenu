#ifndef ItemCommand_H
#define ItemCommand_H

#include "MenuItem.h"

/**
 * @class ItemCommand
 * @brief Represents a menu item that executes a command when selected.
 *
 * The ItemCommand class inherits from MenuItem and allows for the execution
 * of a callback function when the item is entered. This is useful for creating
 * interactive menu items in an LCD menu system.
 *
 * @note The callback function should match the signature defined by the fptr type.
 */
class ItemCommand : public MenuItem {
  private:
    // Declare a function pointer for the command callback.
    fptr callback;

  public:
    /**
     * Construct a new ItemCommand object.
     *
     * @param text The text of the item.
     * @param callback A reference to the callback function to be invoked when
     * the item is entered.
     */
    ItemCommand(const char* text, fptr callback) : MenuItem(text), callback(callback) {}

    /**
     * Get the callback function for this item.
     *
     * @return The function pointer to the callback function.
     */
    fptr getCallback() { return callback; }

    /**
     * Set the callback function for this item.
     *
     * @param callback A reference to the new callback function to be invoked
     * when the item is entered.
     */
    void setCallBack(fptr callback) { this->callback = callback; };

  protected:
    bool process(LcdMenu* menu, const unsigned char command) override {
        switch (command) {
            case ENTER:
                executeCommand();
                return true;
            default:
                return false;
        }
    }
    void executeCommand() {
        if (callback != NULL) {
            callback();
        }
        LOG(F("ItemCommand::enter"), text);
    }
};

#define ITEM_COMMAND(...) (new ItemCommand(__VA_ARGS__))

#endif  // ITEM_COMMAND_H
