// Creator: @ShishkinDmitriy
#pragma once

#include "BaseWidget.h"

class LcdMenu;

/**
 * @class BaseWidgetValue
 * @brief Base class for widget holding some value.
 * @tparam T The type of value this widget holds
 * @see BaseWidget
 */
template <typename T>
class BaseWidgetValue : public BaseWidget {

  protected:
    T& value;
    const char* format = nullptr;
    void (*callback)(const T) = nullptr;

  public:
    BaseWidgetValue(
        T& value,
        const char* format,
        const uint8_t cursorOffset = 0,
        void (*callback)(const T) = nullptr)
        : BaseWidget(cursorOffset), value(value), format(format), callback(callback) {}
    /**
     * @brief Retrieve current value.
     */
    const T& getValue() const { return value; }
    /**
     * @brief Sets the value.
     *
     * @param newValue The value to set.
     * @note You need to call `LcdMenu::refresh` after this method to see the changes.
     */
    virtual void setValue(const T& newValue) {
        if (value != newValue) {
            value = newValue;
            handleChange();
        }
    }

  protected:
    /**
     * @brief Draw the widget into specified buffer.
     *
     * @param buffer the buffer where widget will be drawn
     * @param start the index where to start drawing in the buffer
     */
    uint8_t draw(char* buffer, const uint8_t start) override {
        if (start >= ITEM_DRAW_BUFFER_SIZE) return 0;
        return snprintf(buffer + start, ITEM_DRAW_BUFFER_SIZE - start, format, value);
    }

    /**
     * @brief Process a command for this widget
     * @param menu Pointer to the menu instance
     * @param command The command to process
     * @return true if the command was handled, false otherwise
     */
    bool process(LcdMenu* menu, unsigned char command) override = 0;

    void handleChange() {
        if (callback != nullptr) {
            callback(value);
        }
    }

  public:
    ~BaseWidgetValue() override = default;
};