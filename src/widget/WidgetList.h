#pragma once

#include "BaseWidgetValue.h"
#include "utils/std.h"
#include <vector>

/**
 * @class WidgetList
 * @brief Widget that allows a user to select a value from a list.
 * Manages a value within a specified list, allowing cycling through values.
 * @tparam T the type of value
 * @tparam V the type of stored value, the type should be fully compatible with `uint8_t` type, meaning
 * all arithmetic operations, cast, assignment should be supported for type `V`. For example, T = char*, V = Ref<uint8_t>.
 *
 * NOTE: Despite the fact that the class has template type <T> the base
 * Widget will have type <uint8_t>. It reflects that the value of this widget
 * is index in the specified list, not value itself. Also be aware that
 * the ItemWidget's callback should have correct type <uint8_t>, not <T>.
 */
template <typename T, typename V = uint8_t>
class WidgetList : public BaseWidgetValue<V> {
  protected:
    const bool cycle;
    const std::vector<T>& values;

  public:
    WidgetList(
        const std::vector<T>& values,
        const V activePosition,
        const char* format,
        const uint8_t cursorOffset,
        const bool cycle,
        std::function<void(const V&)> callback)
        : BaseWidgetValue<V>(activePosition, format, cursorOffset, callback),
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
        LOG(action, (uint8_t)this->value);
    }
    /**
     * @brief Draw the widget into specified buffer.
     *
     * @param buffer the buffer where widget will be drawn
     * @param start the index where to start drawing in the buffer
     */
    uint8_t draw(char* buffer, const uint8_t start) override {
        if (start >= ITEM_DRAW_BUFFER_SIZE) return 0;
        return snprintf(buffer + start, ITEM_DRAW_BUFFER_SIZE - start, this->format, values[(uint8_t)this->value]);
    }
    bool nextValue() {
        if (static_cast<int>(this->value) + 1 < static_cast<int>(values.size())) {
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
        if (static_cast<int>(this->value) > 0) {
            this->value--;
            return true;
        }
        if (cycle) {
            this->value = values.size() - 1;
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
 * @param activePosition The initial active position in the list (default: 0).
 * @param format The format of the value (default: "%s").
 * @param cursorOffset The cursor offset (default: 0).
 * @param cycle Whether to cycle through the list (default: false).
 * @param callback The callback function to call when the value changes (default: nullptr).
 */
template <typename T>
inline BaseWidgetValue<uint8_t>* WIDGET_LIST(
    const std::vector<T>& values,
    const uint8_t activePosition = 0,
    const char* format = "%s",
    const uint8_t cursorOffset = 0,
    const bool cycle = false,
    std::function<void(const uint8_t&)> callback = {}) {
    return new WidgetList<T, uint8_t>(values, activePosition, format, cursorOffset, cycle, callback);
}

/**
 * @brief Function to create a new WidgetList<T> instance.
 * @note Make sure that value reference is not deallocated earlier than this widget.
 * @tparam T The type of the value.
 *
 * @param values The list of values to choose from.
 * @param activePosition Initial active position in the list (this value is passed by reference, so it can be updated externally).
 * @param format The format of the value (default: "%s").
 * @param cursorOffset The cursor offset (default: 0).
 * @param cycle Whether to cycle through the list (default: false).
 * @param callback The callback function to call when the value changes (default: nullptr), parameter of callback will be `Ref<uint8_t>`
 */
template <typename T>
inline BaseWidgetValue<Ref<uint8_t>>* WIDGET_LIST_REF(
    const std::vector<T>& values,
    uint8_t& activePosition,
    const char* format = "%s",
    const uint8_t cursorOffset = 0,
    const bool cycle = false,
    std::function<void(const Ref<uint8_t>&)> callback = {}) {
    return new WidgetList<T, Ref<uint8_t>>(values, Ref<uint8_t>(activePosition), format, cursorOffset, cycle, callback);
}