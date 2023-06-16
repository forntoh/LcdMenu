/**
 * ---
 *
 * # ItemList
 *
 * This item type indicates that the current item is a **list**.
 *
 * ### Functionality
 * - `left()`: cycles down the list
 * - `right()`: cycles up the list. This function can be used alone with a
 * single button. Once the menu reaches the end of the list, it automatically
 * starts back from the beginning.
 * - `enter()`: invokes the command *(callback)* bound to this item.
 *
 * ### Usage
 * This item can be used for other primitive data types; you just need to pass
 * it as a string, then parse the result to the desired datatype.
 *
 * ### Parameters
 * @param key: key of the items
 * @param items: array of items to display
 * @param itemCount: number of items in `items`
 * @param callback: reference to a callback function that will be invoked
 * when the user selects an item from the list.
 */
#ifndef ItemList_H
#define ItemList_H
#include "MenuItem.h"

class ItemList : public MenuItem {
   private:
    fptrInt callback = NULL;  ///< Pointer to a callback function
    String* items = NULL;     ///< Pointer to an array of items
    uint8_t itemIndex = 0;    ///< The current selected item index
    const uint8_t itemCount;  ///< The total number of items in the list

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
    ItemList(const char* key, String* items, const uint8_t itemCount,
             fptrInt callback)
        : MenuItem(key, MENU_ITEM_LIST),
          itemCount(itemCount),
          callback(callback),
          items(items) {}

    /**
     * @brief Returns the index of the currently selected item.
     *
     * @return The index of the currently selected item.
     */
    uint8_t getItemIndex() override { return itemIndex; }

    /**
     * @brief Returns the total number of items in the list.
     *
     * @return The total number of items in the list.
     */
    uint8_t getItemCount() override { return itemCount; };

    /**
     * @brief Returns a pointer to the array of items.
     *
     * @return A pointer to the array of items.
     */
    String* getItems() override { return items; }
};

#define ITEM_STRING_LIST(...) (new ItemList(__VA_ARGS__))

#endif