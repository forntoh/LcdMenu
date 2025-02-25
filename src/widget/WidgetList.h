#pragma once

#include "BaseWidgetValue.h"

/**
 * @class WidgetList
 * @brief A templated class for managing a list of widget values.
 *
 * This class extends BaseWidgetValue and provides functionality to manage a list of values
 * with support for cycling through the list and handling commands to navigate the list.
 *
 * NOTE: Despite the fact that the class has template type <T> the base
 * Widget will have type <uint8_t>. It reflects that the value of this widget
 * is index in the specified list, not value itself. Also be aware that
 * the ItemWidget's callback should have correct type <uint8_t>, not <T>.
 *
 * @tparam T The type of the values in the list.
 */
template <typename T>
class WidgetList : public BaseWidgetValue<uint8_t> {
  protected:
    const uint8_t size;  ///< The number of elements in the list.
    const bool cycle;    ///< A boolean indicating whether the list should cycle.
    const T* values;     ///< The array of values in the list.

  public:
    /**
     * @brief Constructor for WidgetList.
     *
     * @param values An array of values of type T.
     * @param size The number of elements in the values array.
     * @param activePosition The initial active position in the list.
     * @param format The format string for displaying the value.
     * @param cursorOffset The offset for the cursor position.
     * @param cycle A boolean indicating whether the list should cycle.
     * @param callback A callback function to be called when the value changes.
     */
    WidgetList(
        const T values[],
        const uint8_t size,
        uint8_t activePosition,
        const char* format,
        const uint8_t cursorOffset,
        const bool cycle,
        void (*callback)(const uint8_t))
        : BaseWidgetValue<uint8_t>(activePosition, format, cursorOffset, callback),
          size(size),
          cycle(cycle),
          values(values) {}

    /**
     * @brief Constructor for WidgetList with Ref<uint8_t> activePosition.
     *
     * @param values An array of values of type T.
     * @param size The number of elements in the values array.
     * @param activePosition A reference to the initial active position in the list.
     * @param format The format string for displaying the value.
     * @param cursorOffset The offset for the cursor position.
     * @param cycle A boolean indicating whether the list should cycle.
     * @param callback A callback function to be called when the value changes.
     */
    WidgetList(
        const T values[],
        const uint8_t size,
        Ref<uint8_t> activePosition,
        const char* format,
        const uint8_t cursorOffset,
        const bool cycle,
        void (*callback)(const uint8_t))
        : BaseWidgetValue<uint8_t>(activePosition, format, cursorOffset, callback),
          size(size),
          cycle(cycle),
          values(values) {}

    /**
     * @brief Constructor for WidgetList with Ptr<uint8_t> activePosition.
     *
     * @param values An array of values of type T.
     * @param size The number of elements in the values array.
     * @param activePosition A pointer to the initial active position in the list.
     * @param format The format string for displaying the value.
     * @param cursorOffset The offset for the cursor position.
     * @param cycle A boolean indicating whether the list should cycle.
     * @param callback A callback function to be called when the value changes.
     */
    WidgetList(
        const T values[],
        const uint8_t size,
        Ptr<uint8_t> activePosition,
        const char* format,
        const uint8_t cursorOffset,
        const bool cycle,
        void (*callback)(const uint8_t))
        : BaseWidgetValue<uint8_t>(activePosition, format, cursorOffset, callback),
          size(size),
          cycle(cycle),
          values(values) {}

    /**
     * @brief Increment the active value in the list.
     *
     * If the active value is less than the size of the list, it increments the value.
     * If cycling is enabled and the active value is at the end of the list, it wraps around to the beginning.
     */
    void nextValue() {
        if (getValue() < size - 1) {
            setValue(getValue() + 1);
            LOG(F("WidgetList::nextValue"), getValue());
        } else if (cycle) {
            setValue(0);
            LOG(F("WidgetList::nextValue"), getValue());
        }
    }

    /**
     * @brief Decrement the active value in the list.
     *
     * If the active value is greater than 0, it decrements the value.
     * If cycling is enabled and the active value is at the beginning of the list, it wraps around to the end.
     */
    void previousValue() {
        if (getValue() > 0) {
            setValue(getValue() - 1);
            LOG(F("WidgetList::previousValue"), getValue());
        } else if (cycle) {
            setValue(size - 1);
            LOG(F("WidgetList::previousValue"), getValue());
        }
    }

  protected:
    /**
     * @brief Process a command to navigate the list.
     *
     * Handles the following commands when in edit mode:
     * - `UP` - increment the value and trigger the callback.
     * - `DOWN` - decrement the value and trigger the callback.
     *
     * @param menu A pointer to the LcdMenu instance.
     * @param command The command to process.
     * @return True if the command was handled, false otherwise.
     */
    bool process(LcdMenu* menu, const unsigned char command) override {
        MenuRenderer* renderer = menu->getRenderer();
        if (renderer->isInEditMode()) {
            switch (command) {
                case UP:
                    nextValue();
                    return true;
                case DOWN:
                    previousValue();
                    return true;
                default:
                    return false;
            }
        }
        return false;
    }

    /**
     * @brief Draw the current value to a buffer.
     *
     * @param buffer The buffer to draw the value into.
     * @param start The starting position in the buffer.
     * @return The number of characters written to the buffer.
     */
    uint8_t draw(char* buffer, const uint8_t start) override {
        if (start >= ITEM_DRAW_BUFFER_SIZE) return 0;
        return snprintf(buffer + start, ITEM_DRAW_BUFFER_SIZE - start, format, values[getValue()]);
    }
};

/**
 * @brief Function to create a new WidgetList<T> object.
 *
 * @param values The list of values to select from
 * @param size The size of the list
 * @param activePosition The initial active position in the list.
 * @param format The format to display the value (default is "%s")
 * @param cursorOffset The offset for the cursor (default is 0)
 * @param cycle Whether the list should cycle (default is false)
 * @param callback The callback function to execute when value changes (default is nullptr)
 */
template <typename T>
inline BaseWidgetValue<uint8_t>* WIDGET_LIST(
    const T values[],
    const uint8_t size,
    uint8_t activePosition,
    const char* format = "%s",
    const uint8_t cursorOffset = 0,
    const bool cycle = false,
    void (*callback)(const uint8_t) = nullptr) {
    return new WidgetList<T>(values, size, activePosition, format, cursorOffset, cycle, callback);
}

/**
 * @brief Creates a new WidgetList with the given parameters.
 *
 * @tparam T The type of the values in the list.
 * @param values An array of values to be displayed in the widget list.
 * @param size The number of elements in the values array.
 * @param activePosition Initial active position in the list (this value is passed by reference, so it can be updated externally).
 * @param format A format string for displaying the values (default is "%s").
 * @param cursorOffset The offset for the cursor position (default is 0).
 * @param cycle Whether the list should cycle when reaching the end (default is false).
 * @param callback A callback function to be called when the active position changes (default is nullptr).
 * @return A pointer to a new WidgetList object.
 */
template <typename T>
inline BaseWidgetValue<uint8_t>* WIDGET_LIST(
    const T values[],
    const uint8_t size,
    Ref<uint8_t> activePosition,
    const char* format = "%s",
    const uint8_t cursorOffset = 0,
    const bool cycle = false,
    void (*callback)(const uint8_t) = nullptr) {
    return new WidgetList<T>(values, size, activePosition, format, cursorOffset, cycle, callback);
}

/**
 * @brief Creates a new WidgetList object.
 *
 * @tparam T The type of the values in the list.
 * @param values An array of values to be displayed in the widget list.
 * @param size The number of elements in the values array.
 * @param activePosition Initial active position in the list (this value is passed by pointer, so it can be updated externally).
 * @param format A format string for displaying the values (default is "%s").
 * @param cursorOffset The offset for the cursor position (default is 0).
 * @param cycle A boolean indicating whether the list should cycle (default is false).
 * @param callback A callback function to be called when the active position changes (default is nullptr).
 * @return A pointer to a BaseWidgetValue object containing the widget list.
 */
template <typename T>
inline BaseWidgetValue<uint8_t>* WIDGET_LIST(
    const T values[],
    const uint8_t size,
    Ptr<uint8_t> activePosition,
    const char* format = "%s",
    const uint8_t cursorOffset = 0,
    const bool cycle = false,
    void (*callback)(const uint8_t) = nullptr) {
    return new WidgetList<T>(values, size, activePosition, format, cursorOffset, cycle, callback);
}