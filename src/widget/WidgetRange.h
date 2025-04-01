// Creator: @ShishkinDmitriy
#pragma once

#include "BaseWidgetValue.h"
#include <functional>

/**
 * @class WidgetRange
 * @brief Widget that allows user to select a value from a range.
 * Manages a value within a specified range, allowing incrementing and decrementing.
 * @tparam T the type of value
 * @tparam V the type of stored value, the type should be fully compatible with `T` type, meaning
 * all arithmetic operations, cast, assignment should be supported for type `V`. For example, T = int, V = Ref<int>.
 */
template <typename T, typename V = T>
class WidgetRange : public BaseWidgetValue<V> {
  protected:
    const T step;
    const T minValue;
    const T maxValue;
    const bool cycle;

  public:
    WidgetRange(
        const V value,
        const T step,
        const T min,
        const T max,
        const char* format,
        const uint8_t cursorOffset = 0,
        const bool cycle = false,
        std::function<void(const V&)> callback = nullptr)
        : BaseWidgetValue<V>(value, format, cursorOffset, callback),
          step(step),
          minValue(min),
          maxValue(max),
          cycle(cycle) {}

    /**
     * @brief Sets the value.
     * @param newValue The value to set.
     * @note You need to call `LcdMenu::refresh` after this method to see the changes.
     */
    void setValue(const V& newValue) override {
        const T newV = constrain(static_cast<T>(newValue), minValue, maxValue);
        if (this->value != newV) {
            this->value = newV;
            this->handleChange();
        }
    }

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
                    if (increment()) BaseWidgetValue<V>::handleChange();
                    return true;
                case DOWN:
                    if (decrement()) BaseWidgetValue<V>::handleChange();
                    return true;
                default:
                    return false;
            }
        }
        return false;
    }
    /**
     * @brief Increments the value.
     * If the value exceeds `maxValue` and cycling is enabled, the value resets to `minValue`.
     * @return true if incremented or reset (in case of cycle)
     */
    bool increment() {
        T current = static_cast<T>(this->value);
        T newValue = (current + step > maxValue) ? (cycle ? minValue : maxValue) : (current + step);
        if (newValue != current) {
            this->value = newValue;
            LOG(F("WidgetRange::increment"), newValue);
            return true;
        }
        return false;
    }

    /**
     * @brief Decrements the value.
     * If the value falls below `minValue` and cycling is enabled, the value resets to `maxValue`.
     * @return true if decremented or reset (in case of cycle)
     */
    bool decrement() {
        T current = static_cast<T>(this->value);
        T newValue = (current < minValue + step) ? (cycle ? maxValue : minValue) : (current - step);
        if (newValue != current) {
            this->value = newValue;
            LOG(F("WidgetRange::decrement"), newValue);
            return true;
        }
        return false;
    }
    uint8_t draw(char* buffer, const uint8_t start) override {
        if (start >= ITEM_DRAW_BUFFER_SIZE) return 0;
        return snprintf(buffer + start, ITEM_DRAW_BUFFER_SIZE - start, this->format, static_cast<T>(this->value));
    }
};

/**
 * @brief Function to create a new WidgetRange<T> instance.
 * @tparam T The type of the value.
 *
 * @param value The initial value of the widget.
 * @param step The step value for incrementing/decrementing.
 * @param min The minimum value of the range.
 * @param max The maximum value of the range.
 * @param format The format string for displaying the value.
 * @param cursorOffset The offset for the cursor (default is 0).
 * @param cycle Whether the value should cycle when out of range (default is false).
 * @param callback The callback function to call when the value changes (default is nullptr).
 */
template <typename T>
inline BaseWidgetValue<T>* WIDGET_RANGE(
    const T value,
    const T step,
    const T min,
    const T max,
    const char* format,
    const uint8_t cursorOffset = 0,
    const bool cycle = false,
    std::function<void(const T&)> callback = nullptr) {
    return new WidgetRange<T, T>(value, step, min, max, format, cursorOffset, cycle, callback);
}

/**
 * @brief Function to create a new WidgetRange<T> instance.
 * @note Make sure that value reference is not deallocated earlier than this widget.
 * @tparam T The type of the value.
 *
 * @param value The reference value of this widget (this value is passed by reference, so it can be updated externally).
 * @param step The step value for incrementing/decrementing.
 * @param min The minimum value of the range.
 * @param max The maximum value of the range.
 * @param format The format string for displaying the value.
 * @param cursorOffset The offset for the cursor (default is 0).
 * @param cycle Whether the value should cycle when out of range (default is false).
 * @param callback The callback function to call when the value changes (default is nullptr), parameter of callback will be `Ref<T>`
 */
template <typename T>
inline BaseWidgetValue<Ref<T>>* WIDGET_RANGE_REF(
    T& value,
    const T step,
    const T min,
    const T max,
    const char* format,
    const uint8_t cursorOffset = 0,
    const bool cycle = false,
    std::function<void(const Ref<T>&)> callback = nullptr) {
    return new WidgetRange<T, Ref<T>>(Ref<T>(value), step, min, max, format, cursorOffset, cycle, callback);
}
