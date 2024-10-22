// Creator: @ShishkinDmitriy
#ifndef WIDGET_RANGE_H
#define WIDGET_RANGE_H

#include "BaseWidgetValue.h"

/**
 * @class WidgetRange
 * @brief Widget that allows user to select a value from a range.
 * Manages a value within a specified range, allowing incrementing and decrementing.
 */
template <typename T>
class WidgetRange : public BaseWidgetValue<T> {
  protected:
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
    void setValue(T newValue) override {
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
                    if (increment()) BaseWidgetValue<T>::handleChange();
                    return true;
                case DOWN:
                    if (decrement()) BaseWidgetValue<T>::handleChange();
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
        T newValue = (this->value + step > maxValue) ? (cycle ? minValue : maxValue) : (this->value + step);
        if (newValue != this->value) {
            this->value = newValue;
            printLog(F("WidgetRange::increment"), this->value);
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
        T newValue = (this->value - step < minValue) ? (cycle ? maxValue : minValue) : (this->value - step);
        if (newValue != this->value) {
            this->value = newValue;
            printLog(F("WidgetRange::decrement"), this->value);
            return true;
        }
        return false;
    }
};

/**
 * @brief Function to create a new WidgetRange<float> instance.
 *
 * @param value The initial value of the widget.
 * @param step The step value for incrementing/decrementing.
 * @param min The minimum value of the range.
 * @param max The maximum value of the range.
 * @param format The format string for displaying the value (default is "%2.1f").
 * @param cursorOffset The offset for the cursor (default is 0).
 * @param cycle Whether the value should cycle when out of range (default is false).
 * @param callback The callback function to call when the value changes (default is nullptr).
 */
inline BaseWidgetValue<float>* WIDGET_FLOAT_RANGE(
    float value,
    float step,
    float min,
    float max,
    const char* format = "%2.1f",
    uint8_t cursorOffset = 0,
    bool cycle = false,
    void (*callback)(float) = nullptr) {
    return new WidgetRange<float>(value, step, min, max, format, cursorOffset, cycle, callback);
}

/**
 * @brief Function to create a new WidgetRange<int> instance.
 *
 * @param value The initial value of the widget.
 * @param step The step value for incrementing/decrementing.
 * @param min The minimum value of the range.
 * @param max The maximum value of the range.
 * @param format The format string for displaying the value (default is "%d").
 * @param cursorOffset The offset for the cursor (default is 0).
 * @param cycle Whether the value should cycle when out of range (default is false).
 * @param callback The callback function to call when the value changes (default is nullptr).
 */
inline BaseWidgetValue<int>* WIDGET_INT_RANGE(
    int value,
    int step,
    int min,
    int max,
    const char* format = "%d",
    uint8_t cursorOffset = 0,
    bool cycle = false,
    void (*callback)(int) = nullptr) {
    return new WidgetRange<int>(value, step, min, max, format, cursorOffset, cycle, callback);
}

/**
 * @brief Function to create a new WidgetRange<uint8_t> instance.
 *
 * @param value The initial value of the widget.
 * @param step The step value for incrementing/decrementing.
 * @param min The minimum value of the range.
 * @param max The maximum value of the range.
 * @param format The format string for displaying the value (default is "%d").
 * @param cursorOffset The offset for the cursor (default is 0).
 * @param cycle Whether the value should cycle when out of range (default is false).
 * @param callback The callback function to call when the value changes (default is nullptr).
 */
inline BaseWidgetValue<uint8_t>* WIDGET_UINT8_RANGE(
    uint8_t value,
    uint8_t step,
    uint8_t min,
    uint8_t max,
    const char* format = "%d",
    uint8_t cursorOffset = 0,
    bool cycle = false,
    void (*callback)(uint8_t) = nullptr) {
    return new WidgetRange<uint8_t>(value, step, min, max, format, cursorOffset, cycle, callback);
}

#endif