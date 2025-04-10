#pragma once

#include <ItemWidget.h>
#include <widget/WidgetBool.h>

/**
 * @brief ItemBool class that allows a user to toggle a boolean value.
 *
 * This class is a specialization of the ItemWidget class, which manages a boolean value.
 *
 * @note This is just a wrapper around the ItemWidget class. The same functionality can be achieved
 * by using the ItemWidget class directly with a WidgetBool as the widget.
 *
 * @tparam V the type of the stored value, which should be fully compatible with `bool` type
 */
template <typename V = bool>
class ItemBool : public ItemWidget<V> {
  public:
    virtual ~ItemBool() = default;

    ItemBool(
        const char* text,
        V value,
        const char* textOn,
        const char* textOff,
        const char* format,
        const uint8_t cursorOffset,
        typename ItemWidget<V>::CallbackType callback)
        : ItemWidget<V>(text, new WidgetBool<V>(value, textOn, textOff, format, cursorOffset), callback) {}
};

/**
 * @brief Create a new item that toggles a boolean value.
 *
 * This item allows the user to toggle between two states (true/false).
 *
 * @param text the text to display for the item
 * @param value the initial boolean value
 * @param textOn the text to display when the value is true
 * @param textOff the text to display when the value is false
 * @param callback the callback function to call when the value changes
 * @param format the format string to use when displaying the value
 * @param cursorOffset the offset for the cursor position
 */
inline ItemBool<bool>* ITEM_BOOL(
    const char* text,
    const bool value,
    const char* textOn,
    const char* textOff,
    void (*callback)(const bool),
    const char* format = "%s",
    const uint8_t cursorOffset = 0) {
    return new ItemBool<bool>(text, value, textOn, textOff, format, cursorOffset, callback);
}

/**
 * @brief Create a new item that toggles a boolean value with a reference.
 *
 * This item allows the user to toggle between two states (true/false) and uses a reference
 * to track the current value.
 *
 * @param text the text to display for the item
 * @param value the reference to the boolean value (this value is passed by reference, so it can be updated externally)
 * @param textOn the text to display when the value is true
 * @param textOff the text to display when the value is false
 * @param callback the callback function to call when the value changes
 * @param format the format string to use when displaying the value
 * @param cursorOffset the offset for the cursor position
 */
inline ItemBool<Ref<bool>>* ITEM_BOOL_REF(
    const char* text,
    bool& value,
    const char* textOn,
    const char* textOff,
    void (*callback)(const Ref<bool>),
    const char* format = "%s",
    const uint8_t cursorOffset = 0) {
    return new ItemBool<Ref<bool>>(text, Ref<bool>(value), textOn, textOff, format, cursorOffset, callback);
}