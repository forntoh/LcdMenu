#pragma once

#include <ItemWidget.h>
#include <widget/WidgetList.h>

/**
 * @brief ItemList class that allows a user to select a value from a list.
 *
 * This class is a specialization of the ItemWidget class, which manages a value within a specified list.
 * It allows cycling through values and provides a callback mechanism for when the value changes.
 *
 * @note This is just a wrapper around the ItemWidget class. The same functionality can be achieved
 * by using the ItemWidget class directly with a WidgetList as the widget.
 *
 * @tparam T the type of the values in the list
 * @tparam V the type of the stored value, which should be fully compatible with `uint8_t`
 */
template <typename T, typename V = uint8_t>
class ItemList : public ItemWidget<V> {

  public:
    virtual ~ItemList() = default;

    ItemList(
        const char* text,
        const std::vector<T>& values,
        const V activePosition,
        const char* format,
        const uint8_t cursorOffset,
        const bool cycle,
        typename ItemWidget<V>::CallbackType callback) : ItemWidget<V>(text,
                                                                       new WidgetList<T, V>(values, activePosition, format, cursorOffset, cycle, {}),
                                                                       callback) {}
};

/**
 * @brief Create a new item that displays a list of values.
 *
 * This item allows the user to select a value from a list of options.
 *
 * @tparam T the type of the values in the list
 * @param text the text to display for the item
 * @param values the list of values to display
 * @param callback the callback function to call when the value changes
 * @param activePosition the index of the currently selected value
 * @param format the format string to use when displaying the values
 * @param cursorOffset the offset for the cursor position
 * @param cycle whether to cycle through the list when reaching the end
 */
template <typename T>
inline ItemList<T, uint8_t>* ITEM_LIST(
    const char* text,
    const std::vector<T>& values,
    std::function<void(const uint8_t)> callback,
    const uint8_t activePosition = 0,
    const char* format = "%s",
    const uint8_t cursorOffset = 0,
    const bool cycle = false) {
    return new ItemList<T, uint8_t>(text, values, activePosition, format, cursorOffset, cycle, callback);
}

/**
 * @brief Create a new item that displays a list of values with a reference to the active position.
 *
 * This item allows the user to select a value from a list of options, and it uses a reference
 * to the active position to keep track of the currently selected value.
 *
 * @tparam T the type of the values in the list
 * @param text the text to display for the item
 * @param values the list of values to display
 * @param callback the callback function to call when the value changes
 * @param activePosition a reference to the index of the currently selected value (this value is passed by reference, so it can be updated externally)
 * @param format the format string to use when displaying the values
 * @param cursorOffset the offset for the cursor position
 * @param cycle whether to cycle through the list when reaching the end
 */
template <typename T>
inline ItemList<T, Ref<uint8_t>>* ITEM_LIST_REF(
    const char* text,
    const std::vector<T>& values,
    std::function<void(const Ref<uint8_t>)> callback,
    uint8_t& activePosition,
    const char* format = "%s",
    const uint8_t cursorOffset = 0,
    const bool cycle = false) {
    return new ItemList<T, Ref<uint8_t>>(text, values, Ref<uint8_t>(activePosition), format, cursorOffset, cycle, callback);
}