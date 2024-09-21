/**
 * ---
 *
 * # ItemList
 *
 * This item type represents a **list**.
 *
 * ### Functionality
 * - `down()`: cycles down the list.
 * - `up()`: cycles up the list. This function can be used with a single button.
 *           Once the menu reaches the end of the list, it automatically
 *           starts back from the beginning.
 * - `enter()`: invokes the command (callback) bound to this item.
 *
 * ### Usage
 * This item can be used for other primitive data types; you just need to pass
 * them as strings, then parse the result to the desired datatype.
 *
 * ### Parameters
 * @param key: key of the item.
 * @param items: array of items to display.
 * @param itemCount: number of items in `items`.
 * @param callback: reference to a callback function that will be invoked
 * when the user selects an item from the list.
 */
#ifndef ItemList_H
#define ItemList_H
#include "LcdMenu.h"
#include "MenuItem.h"
#include <utils/utils.h>

class ItemList : public MenuItem {
  private:
    fptrInt callback = NULL;  ///< Pointer to a callback function
    String* items = NULL;     ///< Pointer to an array of items
    const uint8_t itemCount;  ///< The total number of items in the list
    uint16_t itemIndex = 0;   ///< The current selected item index

  public:
    /**
     * @brief Constructs a new ItemList object.
     *
     * @param key The key of the menu item.
     * @param items The array of items to display.
     * @param itemCount The number of items in the array.
     * @param callback A pointer to the callback function to execute when
     * this menu item is selected.
     */
    ItemList(const char* key, String* items, const uint8_t itemCount, fptrInt callback)
        : MenuItem(key), callback(callback), items(items), itemCount(itemCount) {}

    /**
     * @brief Returns the index of the currently selected item.
     *
     * @return The index of the currently selected item.
     */
    uint16_t getItemIndex() { return itemIndex; }

    /**
     * @brief Changes the index of the current item.
     *
     * @return The index of the item to be selected.
     */
    void setItemIndex(uint16_t itemIndex) {
        this->itemIndex = constrain(itemIndex, 0, (uint16_t)(itemCount)-1);
    }

    /**
     * @brief Get the callback bound to the current item.
     *
     * @return The the callback bound to the current item.
     */
    fptrInt getCallbackInt() { return callback; }

    /**
     * @brief Returns the total number of items in the list.
     *
     * @return The total number of items in the list.
     */
    uint8_t getItemCount() { return itemCount; };

    /**
     * @brief Returns a pointer to the array of items.
     *
     * @return A pointer to the array of items.
     */
    String* getItems() { return items; }

    /**
     * @brief Returns the value of the currently selected item.
     *
     * @return The value of the currently selected item.
     */
    char* getValue() {
        return (char*)items[itemIndex].c_str();
    }

  protected:
    void draw(DisplayInterface* display, uint8_t row) override {
        uint8_t maxCols = display->getMaxCols();
        static char* buf = new char[maxCols];
        substring(items[itemIndex].c_str(), 0, maxCols - strlen(text) - 2, buf);
        display->drawItem(row, text, ':', buf);
    }

    bool process(LcdMenu* menu, const unsigned char command) override {
        DisplayInterface* display = menu->getDisplay();
        if (display->getEditModeEnabled()) {
            switch (command) {
                case BACK:
                    display->setEditModeEnabled(false);
                    if (callback != NULL) {
                        callback(itemIndex);
                    }
                    printLog(F("ItemList::exitEditMode"), getValue());
                    return true;
                case UP:
                    selectNext(display);
                    return true;
                case DOWN:
                    selectPrevious(display);
                    return true;
                default:
                    return false;
            }
        } else {
            switch (command) {
                case ENTER:
                    display->setEditModeEnabled(true);
                    printLog(F("ItemList::enterEditMode"), getValue());
                    return true;
                default:
                    return false;
            }
        }
    }

    void selectPrevious(DisplayInterface* display) {
        uint8_t previousIndex = itemIndex;
        itemIndex = constrain(itemIndex - 1, 0, (uint16_t)(itemCount)-1);
        if (previousIndex != itemIndex) {
            MenuItem::draw(display);
        }
        printLog(F("ItemList::selectPrevious"), getValue());
    };

    void selectNext(DisplayInterface* display) {
        uint8_t previousIndex = itemIndex;
        itemIndex = constrain((itemIndex + 1) % itemCount, 0, (uint16_t)(itemCount)-1);
        if (previousIndex != itemIndex) {
            MenuItem::draw(display);
        }
        printLog(F("ItemList::selectNext"), getValue());
    };
};

#define ITEM_STRING_LIST(...) (new ItemList(__VA_ARGS__))

#endif
