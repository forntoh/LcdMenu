// Creator: @ShishkinDmitriy
#ifndef WIDGET_RANGE_H
#define WIDGET_RANGE_H

#include "Widget.h"

template <typename T>
class WidgetRange : public Widget<T> {
  protected:
    const T minValue;
    const T maxValue;
    const bool cycle = false;

  public:
    WidgetRange(
        T value,
        const T step,
        const T min,
        const T max,
        const char* format,
        const uint8_t blinkerOffset = 0,
        const bool cycle = false,
        void (*callback)(T) = nullptr)
        : Widget<T>(value, step, format, blinkerOffset, callback),
          minValue(min),
          maxValue(max),
          cycle(cycle) {}
    /**
     * @brief Sets the value.
     * @param newValue The value to set.
     * @note You need to call `LcdMenu::refresh` after this method to see the changes.
     */
    void setValue(T newValue) override {
        Widget<T>::setValue(constrain(newValue, minValue, maxValue));
    }

  protected:
    /**
     * @brief Increments the value.
     * If value is out of range and `cycle` then `minValue` will be used.
     * If value is out of range and not `cycle` then do nothing.
     * @return true if incremented or reset (in case of cycle)
     */
    bool increment() override {
        if (this->value + this->step > maxValue) {
            if (cycle) {
                this->value = minValue;
                return true;
            }
            return false;
        }
        return Widget<T>::increment();
    }

    /**
     * @brief Decrements the value.
     * If value is out of range and `cycle` then `maxValue` will be used.
     * If value is out of range and not `cycle` then do nothing.
     * @return true if incremented or reset (in case of cycle)
     */
    bool decrement() override {
        if (this->value - this->step < minValue) {
            if (cycle) {
                this->value = maxValue;
                return true;
            }
            return false;
        }
        return Widget<T>::decrement();
    }
};

/**
 * @brief Macro to create a new WidgetRange<float> instance.
 *
 * @param value The initial value of the widget.
 * @param step The step value for incrementing/decrementing.
 * @param min The minimum value of the range.
 * @param max The maximum value of the range.
 * @param format The format string for displaying the value.
 * @param blinkerOffset The offset for the blinker (default is 0).
 * @param cycle Whether the value should cycle when out of range (default is false).
 * @param callback The callback function to call when the value changes (default is nullptr).
 */
#define WIDGET_FLOAT_RANGE(...) (new WidgetRange<float>(__VA_ARGS__))

/**
 * @brief Macro to create a new WidgetRange<int> instance.
 *
 * @param value The initial value of the widget.
 * @param step The step value for incrementing/decrementing.
 * @param min The minimum value of the range.
 * @param max The maximum value of the range.
 * @param format The format string for displaying the value.
 * @param blinkerOffset The offset for the blinker (default is 0).
 * @param cycle Whether the value should cycle when out of range (default is false).
 * @param callback The callback function to call when the value changes (default is nullptr).
 */
#define WIDGET_INT_RANGE(...) (new WidgetRange<int>(__VA_ARGS__))

/**
 * @brief Macro to create a new WidgetRange<uint8_t> instance.
 *
 * @param value The initial value of the widget.
 * @param step The step value for incrementing/decrementing.
 * @param min The minimum value of the range.
 * @param max The maximum value of the range.
 * @param format The format string for displaying the value.
 * @param blinkerOffset The offset for the blinker (default is 0).
 * @param cycle Whether the value should cycle when out of range (default is false).
 * @param callback The callback function to call when the value changes (default is nullptr).
 *
 * @return A pointer to the newly created WidgetRange<uint8_t> instance.
 */
#define WIDGET_UINT8_RANGE(...) (new WidgetRange<uint8_t>(__VA_ARGS__))

#endif