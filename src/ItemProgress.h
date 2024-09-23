#ifndef ItemProgress_H
#define ItemProgress_H

#include "LcdMenu.h"
#include "MenuItem.h"
#include <utils/utils.h>

/**
 * @class ItemCommand
 * @brief A menu item that represents a progress indicator.
 *
 * The ItemProgress class is a specialized MenuItem that displays and manages a progress value.
 * It supports incrementing and decrementing the progress, as well as setting it directly.
 * The progress can be mapped to a different value using a mapping function, and a callback
 * function can be executed when the item is selected.
 */
class ItemProgress : public MenuItem {
  protected:
    fptrMapping mapping = NULL;  ///< Pointer to a mapping function
    fptrInt callback = NULL;     ///< Pointer to a callback function
    uint16_t progress = 0;       ///< The progress
    uint8_t stepLength = 1;

  public:
    /**
     * @brief Constructs a new ItemProgress object.
     *
     * @param text The text of the menu item.
     * @param start The starting position of the list.
     * @param stepLength The step length for incrementing or decrementing the progress.
     * @param mapping A pointer to the mapping function.
     * @param callback A pointer to the callback function to execute when this menu item is selected.
     */
    ItemProgress(const char* text, uint16_t start, uint8_t stepLength, fptrMapping mapping, fptrInt callback)
        : MenuItem(text),
          mapping(mapping),
          callback(callback),
          progress(start),
          stepLength(stepLength) {}

    ItemProgress(const char* text, uint16_t start, fptrInt callback)
        : ItemProgress(text, start, 1, NULL, callback) {}

    ItemProgress(const char* text, fptrInt callback)
        : ItemProgress(text, 0, 1, NULL, callback) {}

    ItemProgress(const char* text, uint8_t stepLength, fptrMapping mapping, fptrInt callback)
        : ItemProgress(text, 0, stepLength, mapping, callback) {}

    /**
     * @brief Increments the progress of the list.
     */
    bool increment() {
        if (progress >= MAX_PROGRESS) {
            return false;
        }
        progress += stepLength;
        printLog(F("ItemProgress::increment"), getValue());
        return true;
    }

    /**
     * @brief Decrements the progress of the list.
     */
    bool decrement() {
        if (progress <= MIN_PROGRESS) {
            return false;
        }
        progress -= stepLength;
        printLog(F("ItemProgress::decrement"), getValue());
        return true;
    }

    /**
     * Set the progress on the item
     * @param uint16_t progress for the item
     */
    void setProgress(uint16_t value) {
        if (value < MIN_PROGRESS || progress > MAX_PROGRESS) {
            return;
        }
        progress = value;
    }

    /**
     * Return the progress
     */
    uint16_t getProgress() { return progress; }

    /**
     * Return the callback
     */
    fptrInt getCallbackInt() { return callback; }

    /**
     * @brief Returns the value to be displayed.
     *        If there's no mapping, it returns the progress
     *        else it returns the mapped progress
     *
     * @return A pointer to the value of the current list item.
     */
    char* getValue() {
        if (mapping == NULL) {
            static char buf[6];
            itoa(progress, buf, 10);
            return buf;
        }
        return mapping(progress);
    }

  protected:
    void draw(DisplayInterface* display, uint8_t row) override {
        uint8_t maxCols = display->getMaxCols();
        static char* buf = new char[maxCols];
        substring(getValue(), 0, maxCols - strlen(text) - 2, buf);
        display->drawItem(row, text, ':', buf);
    }

    bool process(LcdMenu* menu, const unsigned char command) override {
        DisplayInterface* display = menu->getDisplay();
        if (display->getEditModeEnabled()) {
            switch (command) {
                case BACK:
                    display->setEditModeEnabled(false);
                    if (callback != NULL) {
                        callback(progress);
                    }
                    printLog(F("ItemProgress::exitEditMode"), getValue());
                    return true;
                case UP:
                    if (increment()) {
                        MenuItem::draw(display);
                    }
                    return true;
                case DOWN:
                    if (decrement()) {
                        MenuItem::draw(display);
                    }
                    return true;
                default:
                    return false;
            }
        } else {
            switch (command) {
                case ENTER:
                    display->setEditModeEnabled(true);
                    printLog(F("ItemProgress::enterEditMode"), getValue());
                    return true;
                default:
                    return false;
            }
        }
    }
};

#define ITEM_PROGRESS(...) (new ItemProgress(__VA_ARGS__))

#endif  // ItemProgress_H
