/**
 * @file ItemProgress.h
 * @brief This file contains the declaration of the ItemProgress class.
 */

#ifndef ItemProgress_H
#define ItemProgress_H

#include "MenuItem.h"
#include <utils/utils.h>

/**
 * @class ItemProgress
 * @brief ItemProgress indicates that the current item is a list.
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
     * @param key The key of the menu item.
     * @param start The starting position of the list.
     * @param mapping A pointer to the mapping function.
     * @param callback A pointer to the callback function to execute when this
     * menu item is selected.
     */
    ItemProgress(const char* key, uint16_t start, uint8_t stepLength, fptrMapping mapping, fptrInt callback)
        : MenuItem(key, MENU_ITEM_PROGRESS),
          mapping(mapping),
          callback(callback),
          progress(start),
          stepLength(stepLength) {}

    ItemProgress(const char* key, uint16_t start, fptrInt callback)
        : ItemProgress(key, start, 1, NULL, callback) {}

    ItemProgress(const char* key, fptrInt callback)
        : ItemProgress(key, 0, 1, NULL, callback) {}

    ItemProgress(const char* key, uint8_t stepLength, fptrMapping mapping, fptrInt callback)
        : ItemProgress(key, 0, stepLength, mapping, callback) {}

    /**
     * @brief Increments the progress of the list.
     */
    void increment() {
        if (progress >= MAX_PROGRESS) {
            return;
        }
        progress += stepLength;
    }

    /**
     * @brief Decrements the progress of the list.
     */
    void decrement() {
        if (progress <= MIN_PROGRESS) {
            return;
        }
        progress -= stepLength;
    }

    /**
     * Set the progress on the item
     * @param uint16_t progress for the item
     */
    void setProgress(uint16_t value) {
        if (progress < MIN_PROGRESS || progress > MAX_PROGRESS) {
            return;
        }
        progress = value;
    }

    /**
     * Return the progress
     */
    uint16_t getItemIndex() { return progress; }

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

    void draw(DisplayInterface* display, uint8_t row) override {
        uint8_t maxCols = display->getMaxCols();
        static char* buf = new char[maxCols];
        substring(getValue(), 0, maxCols - strlen(text) - 2, buf);
        display->drawItem(row, text, ':', buf);
    }

    bool process(Context context)  {
        switch (context.command) {
            case ENTER: return enter(context);
            case BACK: return back(context);
            case UP: return up(context);
            case DOWN: return down(context);
            default: return false;
        }
    }

  protected:
    bool enter(Context context) {
        DisplayInterface* display = context.display;
        if (display->getEditModeEnabled()) {
            return false;
        }
        display->setEditModeEnabled(true);
        return true;
    };

    bool back(Context context) {
        DisplayInterface* display = context.display;
        if (!display->getEditModeEnabled()) {
            return false;
        }
        display->setEditModeEnabled(false);
        if (callback != NULL) {
            callback(progress);
        }
        return true;
    };

    bool down(Context context) {
        DisplayInterface* display = context.display;
        if (!display->getEditModeEnabled()) {
            return false;
        }
        uint16_t oldProgress = progress;
        decrement();
        if (progress != oldProgress) {
            printCmd(F("LEFT"), getValue());
            MenuItem::draw(display);
        }
        return true;
    };

    bool up(Context context) {
        DisplayInterface* display = context.display;
        if (!display->getEditModeEnabled()) {
            return false;
        }
        uint16_t oldProgress = progress;
        increment();
        if (progress != oldProgress) {
            printCmd(F("RIGHT"), getValue());
            MenuItem::draw(display);
        }
        return true;
    };
};

#define ITEM_PROGRESS(...) (new ItemProgress(__VA_ARGS__))

#endif  // ItemProgress_H