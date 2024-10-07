#ifndef ItemList_H
#define ItemList_H
#include "LcdMenu.h"
#include "MenuItem.h"

/**
 * @class ItemList
 * @brief A class representing a list of items in a menu.
 *
 * The ItemList class extends the MenuItem class and provides functionality
 * for displaying and interacting with a list of items in a menu. It supports
 * navigation through the items and executing a callback function when an item
 * is selected.
 */
class ItemList : public MenuItem {
  private:
    fptruInt callback = NULL;
    String* items = NULL;
    const uint8_t itemCount;
    uint8_t itemIndex = 0;

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
    ItemList(const char* key, String* items, const uint8_t itemCount, fptruInt callback)
        : MenuItem(key), callback(callback), items(items), itemCount(itemCount) {}

    /**
     * @brief Returns the index of the currently selected item.
     *
     * @return The index of the currently selected item.
     */
    uint8_t getItemIndex() { return itemIndex; }

    /**
     * @brief Changes the index of the current item.
     * @note You need to call `LcdMenu::refresh` after this method to see the changes.
     *
     * @return true if the index was changed successfully, false otherwise.
     */
    bool setItemIndex(uint8_t itemIndex) {
        uint8_t desiredIndex = constrain(itemIndex, 0, itemCount - 1);
        if (desiredIndex != this->itemIndex) {
            this->itemIndex = desiredIndex;
            return true;
        }
        return false;
    }

    /**
     * @brief Get the callback bound to the current item.
     *
     * @return The the callback bound to the current item.
     */
    fptruInt getCallbackInt() { return callback; }

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
    void draw(MenuRenderer* renderer) override {
        uint8_t maxCols = renderer->getMaxCols();
        char buf[maxCols];
        concat(text, ':', buf);
        concat(buf, getValue(), buf);
        renderer->drawItem(buf);
    }

    bool process(LcdMenu* menu, const unsigned char command) override {
        MenuRenderer* renderer = menu->getRenderer();
        if (renderer->isInEditMode()) {
            switch (command) {
                case BACK:
                    renderer->setEditMode(false);
                    draw(renderer);
                    if (callback != NULL) {
                        callback(itemIndex);
                    }
                    printLog(F("ItemList::exitEditMode"), getValue());
                    return true;
                case UP:
                    selectNext(renderer);
                    return true;
                case DOWN:
                    selectPrevious(renderer);
                    return true;
                default:
                    return false;
            }
        } else {
            switch (command) {
                case ENTER:
                    renderer->setEditMode(true);
                    draw(renderer);
                    printLog(F("ItemList::enterEditMode"), getValue());
                    return true;
                default:
                    return false;
            }
        }
    }

    void selectPrevious(MenuRenderer* renderer) {
        uint8_t previousIndex = itemIndex;
        (int8_t)(itemIndex - 1) < 0 ? itemIndex = itemCount - 1 : itemIndex--;
        if (previousIndex != itemIndex) {
            draw(renderer);
        }
        printLog(F("ItemList::selectPrevious"), getValue());
    };

    void selectNext(MenuRenderer* renderer) {
        uint8_t previousIndex = itemIndex;
        itemIndex = constrain((itemIndex + 1) % itemCount, 0, itemCount - 1);
        if (previousIndex != itemIndex) {
            draw(renderer);
        }
        printLog(F("ItemList::selectNext"), getValue());
    };
};

#define ITEM_STRING_LIST(...) (new ItemList(__VA_ARGS__))

#endif
