#pragma once

#include "BaseWidgetValue.h"

/**
 * @class WidgetList
 * @brief Widget that allows a user to select a value from a list.
 * Manages a value within a specified list, allowing cycling through values.
 */
template <typename T>
class WidgetList : public BaseWidgetValue<T> {
  protected:
    const uint8_t size;
    uint8_t activePosition;
    const bool cycle;
    const T* values;

  public:
    WidgetList(
        const T values[],
        const uint8_t size,
        const int8_t activePosition,
        const char* format,
        const uint8_t cursorOffset,
        const bool cycle,
        void (*callback)(const T&))
        : BaseWidgetValue<T>(values[activePosition], format, cursorOffset, callback),
          size(size),
          activePosition(activePosition),
          cycle(cycle),
          values(values) {}

  protected:
    /**
     * @brief Process command.
     *
     * Handle commands:
     * - `UP` - increment value and trigger callback;
     * - `DOWN` - decrement value and trigger callback;
     */
    bool process(LcdMenu* menu, const unsigned char command) override {
        MenuRenderer* renderer = menu->getRenderer();
        if (renderer->isInEditMode()) {
            switch (command) {
                case UP:
                    if (nextValue()) {
                        updateValue(F("WidgetList::nextValue"));
                    }
                    return true;
                case DOWN:
                    if (previousValue()) {
                        updateValue(F("WidgetList::previousValue"));
                    }
                    return true;
                default:
                    return false;
            }
        }
        return false;
    }
    void updateValue(const __FlashStringHelper* action) {
        this->value = values[activePosition];
        BaseWidgetValue<T>::handleChange();
        LOG(action, this->value);
    }
    bool nextValue() {
        if (activePosition + 1 < size) {
            activePosition++;
            return true;
        }
        if (cycle) {
            activePosition = 0;
            return true;
        }
        return false;
    }
    bool previousValue() {
        if (activePosition > 0) {
            activePosition--;
            return true;
        }
        if (cycle) {
            activePosition = size - 1;
            return true;
        }
        return false;
    }
};

/**
 * @brief Function to create a new WidgetList<T> instance.
 * @tparam T The type of the value.
 *
 * @param values The list of values to choose from.
 * @param size The size of the list.
 * @param activePosition The initial active position in the list (default: 0).
 * @param format The format of the value (default: "%s").
 * @param cursorOffset The cursor offset (default: 0).
 * @param cycle Whether to cycle through the list (default: false).
 * @param callback The callback function to call when the value changes (default: nullptr).
 */
template <typename T>
inline BaseWidgetValue<T>* WIDGET_LIST(
    const T values[],
    const uint8_t size,
    const uint8_t activePosition = 0,
    const char* format = "%s",
    const uint8_t cursorOffset = 0,
    const bool cycle = false,
    void (*callback)(const T&) = nullptr) {
    return new WidgetList<T>(values, size, activePosition, format, cursorOffset, cycle, callback);
}