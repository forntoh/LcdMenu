#pragma once

#include "BaseWidgetValue.h"

/**
 * @class WidgetList
 * @brief Widget that allows a user to select a value from a list.
 * Manages a value within a specified list, allowing cycling through values.
 * @tparam T the type of value
 * @tparam V the type of stored value, the type should be fully compatible with `size_t` type, meaning
 * all arifmetic operations, cast, assigment whould be supported for type `V`. For example, T = char*, V = Ref<size_t>.
 *
 * NOTE: Despite the fact that the class has template type <T> the base
 * Widget will have type <size_t>. It reflects that the value of this widget
 * is index in the specified list, not value itself. Also be aware that
 * the ItemWidget's callback should have correct type <size_t>, not <T>.
 */
template <typename T, typename V = size_t>
class WidgetList : public BaseWidgetValue<V> {
  protected:
    const size_t size;
    const bool cycle;
    const T* values;

  public:
    WidgetList(
        const T values[],
        const size_t size,
        const V value,
        const char* format,
        const uint8_t cursorOffset,
        const bool cycle,
        void (*callback)(const V&))
        : BaseWidgetValue<V>(value, format, cursorOffset, callback),
          size(size),
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
        BaseWidgetValue<V>::handleChange();
        LOG(action, (size_t) this->value);
    }
    /**
     * @brief Draw the widget into specified buffer.
     *
     * @param buffer the buffer where widget will be drawn
     * @param start the index where to start drawing in the buffer
     */
    uint8_t draw(char* buffer, const uint8_t start) override {
        if (start >= ITEM_DRAW_BUFFER_SIZE) return 0;
        return snprintf(buffer + start, ITEM_DRAW_BUFFER_SIZE - start, this->format, values[(size_t) this->value]);
    }
    bool nextValue() {
        if (this->value + 1 < size) {
            this->value++;
            return true;
        }
        if (cycle) {
            this->value = 0;
            return true;
        }
        return false;
    }
    bool previousValue() {
        if (this->value > 0) {
            this->value--;
            return true;
        }
        if (cycle) {
            this->value = size - 1;
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
 * @param value The initial active position in the list (default: 0).
 * @param format The format of the value (default: "%s").
 * @param cursorOffset The cursor offset (default: 0).
 * @param cycle Whether to cycle through the list (default: false).
 * @param callback The callback function to call when the value changes (default: nullptr).
 */
template <typename T>
inline WidgetList<T, size_t>* WIDGET_LIST(
    const T values[],
    const size_t size,
    const size_t value,
    const char* format = "%s",
    const uint8_t cursorOffset = 0,
    const bool cycle = false,
    void (*callback)(const size_t&) = nullptr) {
    return new WidgetList<T, size_t>(values, size, value, format, cursorOffset, cycle, callback);
}

/**
 * @brief Function to create a new WidgetList<T> instance.
 * @tparam T The type of the value.
 *
 * @param values The list of values to choose from.
 * @param size The size of the list.
 * @param value The reference value of this widget, the value will be used by reference
 * @param format The format of the value (default: "%s").
 * @param cursorOffset The cursor offset (default: 0).
 * @param cycle Whether to cycle through the list (default: false).
 * @param callback The callback function to call when the value changes (default: nullptr), parameter of callback will be `Ref<size_t>`
 */
template <typename T>
inline WidgetList<T, Ref<size_t>>* WIDGET_LIST_REF(
    const T values[],
    const size_t size,
    size_t& value,
    const char* format = "%s",
    const uint8_t cursorOffset = 0,
    const bool cycle = false,
    void (*callback)(const Ref<size_t>&) = nullptr) {
    return new WidgetList<T, Ref<size_t>>(values, size, Ref<size_t>(value), format, cursorOffset, cycle, callback);
}
