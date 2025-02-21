#pragma once

#include "BaseWidgetValue.h"

/**
 * @class WidgetList
 * @brief Widget that allows a user to select a value from a list.
 * Manages a value within a specified list, allowing cycling through values.
 *
 * NOTE: Despite the fact that the class has template type <T> the base
 * Widget will have type <uint8_t>. It reflects that the value of this widget
 * is index in the specified list, not value itself. Also be aware that
 * the ItemWidget's callback should have correct type <uint8_t>, not <T>.
 */
template <typename T>
class WidgetList : public BaseWidgetValue<uint8_t> {
  protected:
    const uint8_t size;
    const bool cycle;
    const T* values;

  public:
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

    WidgetList(
        const T values[],
        const uint8_t size,
        uint8_t* activePosition,
        const char* format,
        const uint8_t cursorOffset,
        const bool cycle,
        void (*callback)(const uint8_t))
        : BaseWidgetValue<uint8_t>(activePosition, format, cursorOffset, callback),
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
     * @brief Draw the widget into specified buffer.
     *
     * @param buffer the buffer where widget will be drawn
     * @param start the index where to start drawing in the buffer
     */
    uint8_t draw(char* buffer, const uint8_t start) override {
        if (start >= ITEM_DRAW_BUFFER_SIZE || valuePtr == nullptr) return 0;
        return snprintf(buffer + start, ITEM_DRAW_BUFFER_SIZE - start, format, values[*valuePtr]);
    }

    void nextValue() {
        if (getValue() + 1 < size) {
            setValue(getValue() + 1);
            LOG(F("WidgetList::nextValue"), getValue());
            return;
        }
        if (cycle) {
            setValue(0);
            LOG(F("WidgetList::nextValue"), getValue());
        }
    }

    void previousValue() {
        if (getValue() > 0) {
            setValue(getValue() - 1);
            LOG(F("WidgetList::previousValue"), getValue());
            return;
        }
        if (cycle) {
            setValue(size - 1);
            LOG(F("WidgetList::previousValue"), getValue());
        }
    }
};

/**
 * @brief Function to create a new WidgetList<T> instance.
 * @tparam T The type of the value.
 *
 * @param values The list of values to choose from.
 * @param size The size of the list.
 * @param activePosition The initial active position in the list.
 * @param format The format of the value (default: "%s").
 * @param cursorOffset The cursor offset (default: 0).
 * @param cycle Whether to cycle through the list (default: false).
 * @param callback The callback function to call when the value changes (default: nullptr).
 */
template <typename T>
inline WidgetList<T>* WIDGET_LIST(
    const T values[],
    const uint8_t size,
    const uint8_t activePosition,
    const char* format = "%s",
    const uint8_t cursorOffset = 0,
    const bool cycle = false,
    void (*callback)(uint8_t) = nullptr) {
    return new WidgetList<T>(values, size, activePosition, format, cursorOffset, cycle, callback);
}

/**
 * @brief Function to create a new WidgetList<T> instance.
 * @tparam T The type of the value.
 *
 * @param values The list of values to choose from.
 * @param size The size of the list.
 * @param activePosition Reference to the active position in the list, which can be updated externally.
 * @param format The format of the value (default: "%s").
 * @param cursorOffset The cursor offset (default: 0).
 * @param cycle Whether to cycle through the list (default: false).
 * @param callback The callback function to call when the value changes (default: nullptr).
 */
template <typename T>
inline WidgetList<T>* WIDGET_LIST_REF(
    const T values[],
    const uint8_t size,
    uint8_t& activePosition,
    const char* format = "%s",
    const uint8_t cursorOffset = 0,
    const bool cycle = false,
    void (*callback)(uint8_t) = nullptr) {
    return new WidgetList<T>(values, size, &activePosition, format, cursorOffset, cycle, callback);
}
