/**
 * @file ItemProgress.h
 * @brief This file contains the declaration of the ItemProgress class.
 */

#ifndef ItemProgress_H
#define ItemProgress_H

#include "MenuItem.h"

#define MIN 0
#define MAX 100

/**
 * @class ItemProgress
 * @brief ItemProgress indicates that the current item is a list.
 */
class ItemProgress : public MenuItem {
   private:
    fptrMapping mapping = NULL;  ///< Pointer to a callback function
    fptrInt callback = NULL;     ///< Pointer to a callback function
    uint8_t start = 0;           ///< The total number of items in the list
    uint8_t progress = 0;        ///< The total number of items in the list
    uint8_t precision = 1;

   public:
    /**
     * @brief Constructs a new ItemProgress object.
     *
     * @param key The key of the menu item.
     * @param start The starting position of the list.
     * @param mapping A pointer to the mapping function.
     * @param callback A pointer to the callback function to execute when this
     * menu item is selected.
     */
    ItemProgress(const char* key, uint8_t start, uint8_t precision,
                 fptrMapping mapping, fptrInt callback)
        : MenuItem(key, MENU_ITEM_PROGRESS),
          mapping(mapping),
          callback(callback),
          start(start),
          precision(precision) {}

    ItemProgress(const char* key, uint8_t start, fptrInt callback)
        : ItemProgress(key, start, 1, NULL, callback) {}

    ItemProgress(const char* key, fptrInt callback)
        : ItemProgress(key, 0, 1, NULL, callback) {}

    ItemProgress(const char* key, uint8_t precision, fptrMapping mapping,
                 fptrInt callback)
        : ItemProgress(key, 0, precision, mapping, callback) {}

    /**
     * @brief Increments the progress of the list.
     */
    void increment() override {
        progress = constrain(progress + precision, MIN, MAX);
    }

    /**
     * @brief Decrements the progress of the list.
     */
    void decrement() override {
        progress = constrain(progress - precision, MIN, MAX);
    }

    /**
     * Return the progress
     */
    uint8_t getItemIndex() override { return progress; }

    /**
     * @brief Returns the value to be displayed.
     *        If there's no mapping, it returns the progress
     *        else it returns the mapped progress
     *
     * @return A pointer to the value of the current list item.
     */
    char* getValue() override {
        if (mapping == NULL) {
            static char buf[4];
            itoa(progress, buf, 10);
            return buf;
        } else {
            return mapping(progress);
        }
    }
};

#define ITEM_PROGRESS(...) (new ItemProgress(__VA_ARGS__))

#endif  // ItemProgress_H