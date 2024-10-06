#ifndef ItemRangeBase_H
#define ItemRangeBase_H

#include "LcdMenu.h"
#include "MenuItem.h"
#include <utils/utils.h>

template <typename T>
/**
 * @brief Item that allows user to select a value from a range.
 *        It can be used to renderer all sorts of values that can be incremented or decremented.
 *        You can additionally pass a unit to be displayed after the value. e.g. "%", "°C", "°F" etc.
 *
 * ```
 * ┌──────────────────────────────────┐
 * │ > T E X T : V A L U E U N I T    │
 * └──────────────────────────────────┘
 * ```
 *
 * Additionally to `text` this item has float `currentValue`.
 * Has internal `edit` state.
 */
class ItemRangeBase : public MenuItem {
  protected:
    const T minValue;
    const T maxValue;
    T currentValue;
    void (*callback)(T);
    const char* unit;
    const T step;
    bool commitOnChange;

  public:
    /**
     * @brief Construct a new Item Range Base object
     *
     * @param text The text of the item.
     * @param min The minimum value.
     * @param max The maximum value.
     * @param startingValue The current value.
     * @param callback A pointer to the callback function to execute when this menu item is selected.
     * @param unit The unit e.g. "%", "°C", "°F".
     * @param step The step value for increment/decrement.
     * @param commitOnChange If true, the callback will be called every time the value changes.
     */
    ItemRangeBase(
        const char* text,
        const T min,
        const T max,
        T startingValue,
        void (*callback)(T),
        const char* unit = NULL,
        T step = 1,
        bool commitOnChange = false)
        : MenuItem(text), minValue(min), maxValue(max), currentValue(startingValue), callback(callback), unit(unit), step(step), commitOnChange(commitOnChange) {}

    /**
     * @brief Increments the value.
     */
    bool increment() {
        if (currentValue + step > maxValue) {
            return false;
        }
        currentValue += step;
        printLog(F("ItemRangeBase::increment"), currentValue);
        return true;
    }

    /**
     * @brief Decrements the value.
     */
    bool decrement() {
        if (currentValue - step < minValue) {
            return false;
        }
        currentValue -= step;
        printLog(F("ItemRangeBase::decrement"), currentValue);
        return true;
    }

    /**
     * @brief Sets the value.
     *
     * @param value The value to set.
     * @note You need to call `LcdMenu::refresh` after this method to see the changes.
     * @return true if the value was set successfully, false otherwise.
     */
    bool setCurrentValue(T value) {
        if (value < minValue || value > maxValue || value == currentValue) {
            return false;
        }
        currentValue = value;
        return true;
    }

    /**
     * @brief Returns the current value.
     * @return The current value.
     */
    T getCurrentValue() { return currentValue; }

    /**
     * @brief Returns the value to be displayed.
     *        If a unit is provided, it will be concatenated to the value.
     * @return The value to be displayed.
     */
    virtual char* getDisplayValue() = 0;

  protected:
    void draw(MenuRenderer* renderer, uint8_t itemIndex, uint8_t screenRow) override {
        uint8_t maxCols = renderer->getMaxCols();
        char buf[maxCols];
        concat(text, ':', buf);
        concat(buf, getDisplayValue(), buf);
        renderer->drawItem(itemIndex, screenRow, buf);
    }

    bool process(LcdMenu* menu, const unsigned char command) override {
        MenuRenderer* renderer = menu->getRenderer();
        if (renderer->isInEditMode()) {
            switch (command) {
                case BACK:
                    renderer->setEditMode(false);
                    MenuItem::draw(renderer);
                    if (callback != NULL && !commitOnChange) {
                        callback(currentValue);
                    }
                    printLog(F("ItemRangeBase::exitEditMode"), currentValue);
                    return true;
                case UP:
                    if (increment()) {
                        MenuItem::draw(renderer);
                        if (commitOnChange && callback != NULL) {
                            callback(currentValue);
                        }
                    }
                    return true;
                case DOWN:
                    if (decrement()) {
                        MenuItem::draw(renderer);
                        if (commitOnChange && callback != NULL) {
                            callback(currentValue);
                        }
                    }
                    return true;
                default:
                    return false;
            }
        } else {
            switch (command) {
                case ENTER:
                    renderer->setEditMode(true);
                    MenuItem::draw(renderer);
                    printLog(F("ItemRangeBase::enterEditMode"), currentValue);
                    return true;
                default:
                    return false;
            }
        }
    }
};

#endif  // ItemRangeBase_H