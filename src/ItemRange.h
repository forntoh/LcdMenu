#pragma once

#include <ItemWidget.h>
#include <widget/WidgetRange.h>

/**
 * @brief ItemRange class that allows a user to select a value from a range.
 *
 * This class is a specialization of the ItemWidget class, which manages a value within a specified range.
 *
 * @note This is just a wrapper around the ItemWidget class. The same functionality can be achieved
 * by using the ItemWidget class directly with a WidgetRange as the widget.
 *
 * @tparam T the type of the values in the list
 * @tparam V the type of the stored value, which should be fully compatible with `T`
 */
template <typename T, typename V = T>
class ItemRange : public ItemWidget<V> {

  public:
    ItemRange(
        const char* text,
        const V value,
        const T step,
        const T min,
        const T max,
        const char* format,
        const uint8_t cursorOffset,
        const bool cycle,
        typename ItemWidget<V>::CallbackType callback) : ItemWidget<V>(text,
                                                                       new WidgetRange<T, V>(value, step, min, max, format, cursorOffset, cycle),
                                                                       callback) {}
};

/**
 * @brief Create a new item that displays a range of values.
 *
 * This item allows the user to select a value from a range of options.
 *
 * @tparam T the type of the values in the range
 * @param text the text to display for the item
 * @param value the initial value to display
 * @param step the step size for incrementing/decrementing the value
 * @param min the minimum value in the range
 * @param max the maximum value in the range
 * @param callback the callback function to call when the value changes
 * @param format the format string to use when displaying the values
 * @param cursorOffset the offset for the cursor position
 * @param cycle whether to cycle through the range when reaching the end
 */
template <typename T>
inline ItemRange<T, T>* ITEM_RANGE(
    const char* text,
    const T value,
    const T step,
    const T min,
    const T max,
    void (*callback)(const T),
    const char* format = "%s",
    const uint8_t cursorOffset = 0,
    const bool cycle = false) {
    return new ItemRange<T, T>(text, value, step, min, max, format, cursorOffset, cycle, callback);
}

/**
 * @brief Create a new item that displays a range of values.
 *
 * This item allows the user to select a value from a range of options.
 *
 * @tparam T the type of the values in the range
 * @param text the text to display for the item
 * @param value the reference value to display (this value is passed by reference, so it can be updated externally)
 * @param step the step size for incrementing/decrementing the value
 * @param min the minimum value in the range
 * @param max the maximum value in the range
 * @param callback the callback function to call when the value changes
 * @param format the format string to use when displaying the values
 * @param cursorOffset the offset for the cursor position
 * @param cycle whether to cycle through the range when reaching the end
 */
template <typename T>
inline ItemRange<T, Ref<T>>* ITEM_RANGE_REF(
    const char* text,
    T& value,
    const T step,
    const T min,
    const T max,
    void (*callback)(const Ref<T>),
    const char* format = "%s",
    const uint8_t cursorOffset = 0,
    const bool cycle = false) {
    return new ItemRange<T, Ref<T>>(text, Ref<T>(value), step, min, max, format, cursorOffset, cycle, callback);
}