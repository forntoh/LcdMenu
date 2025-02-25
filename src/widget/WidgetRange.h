// Creator: @ShishkinDmitriy
#pragma once

#include "BaseWidgetValue.h"

/**
 * @class WidgetRange
 * @brief Widget that allows user to select a value from a range.
 * Manages a value within a specified range, allowing incrementing and decrementing.
 */
template <typename T>
class WidgetRange : public BaseWidgetValue<T> {
  private:
    const T step;
    const T minValue;
    const T maxValue;
    const bool cycle;

  public:
    WidgetRange(
        T value,
        const T step,
        const T min,
        const T max,
        const char* format,
        const uint8_t cursorOffset = 0,
        const bool cycle = false,
        void (*callback)(T) = nullptr)
        : BaseWidgetValue<T>(value, format, cursorOffset, callback),
          step(step),
          minValue(min),
          maxValue(max),
          cycle(cycle) {}
    /**
     * @brief Sets the value.
     * @param newValue The value to set.
     * @note You need to call `LcdMenu::refresh` after this method to see the changes.
     */
    void setValue(const T& newValue) override {
        BaseWidgetValue<T>::setValue(constrain(newValue, minValue, maxValue));
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
                    increment();
                    return true;
                case DOWN:
                    decrement();
                    return true;
                default:
                    return false;
            }
        }
        return false;
    }

  private:
    /**
     * @brief Increments the value.
     * If the value exceeds `maxValue` and cycling is enabled, the value resets to `minValue`.
     */
    void increment() {
        T newValue = (this->getValue() + step > maxValue) ? (cycle ? minValue : maxValue) : (this->getValue() + step);
        if (newValue != this->getValue()) {
            this->setValue(newValue);
            LOG(F("WidgetRange::increment"), this->getValue());
        }
    }

    /**
     * @brief Decrements the value.
     * If the value falls below `minValue` and cycling is enabled, the value resets to `maxValue`.
     */
    void decrement() {
        T newValue = (this->getValue() < minValue + step) ? (cycle ? maxValue : minValue) : (this->getValue() - step);
        if (newValue != this->getValue()) {
            this->setValue(newValue);
            LOG(F("WidgetRange::decrement"), this->getValue());
        }
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
    T value,
    T step,
    T min,
    T max,
    const char* format,
    uint8_t cursorOffset = 0,
    bool cycle = false,
    void (*callback)(T) = nullptr) {
    return new WidgetRange<T>(value, step, min, max, format, cursorOffset, cycle, callback);
}