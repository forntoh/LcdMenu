// Creator: @ShishkinDmitriy
#ifndef BASE_WIDGET_VALUE_H
#define BASE_WIDGET_VALUE_H

#include "BaseWidget.h"

class LcdMenu;

/**
 * @class BaseWidgetValue
 * @brief Base class for widget holding some value.
 */
template <typename T>
class BaseWidgetValue : public BaseWidget {
    template <typename T0, typename T1, typename T2>
    friend class Item;

  protected:
    T value;
    const char* format = nullptr;
    void (*callback)(T) = nullptr;

  public:
    BaseWidgetValue(T value, const char* format, const uint8_t cursorOffset = 0, void (*callback)(T) = nullptr)
        : BaseWidget(cursorOffset), value(value), format(format), callback(callback) {}
    /**
     * @brief Retrieve current value.
     */
    T getValue() const { return value; }
    /**
     * @brief Sets the value.
     *
     * @param newValue The value to set.
     * @note You need to call `LcdMenu::refresh` after this method to see the changes.
     */
    virtual void setValue(const T newValue) { value = newValue; }

  protected:
    /**
     * @brief Draw the widget into specified buffer.
     *
     * @param buffer the buffer where widget will be drawn
     * @param start the index where to start drawing in the buffer
     */
    uint8_t draw(char* buffer, const uint8_t start) override {
        return snprintf(buffer + start, ITEM_DRAW_BUFFER_SIZE, format, value);
    }

    bool process(LcdMenu* menu, unsigned char command) override = 0;

    void handleChange() {
        if (callback != nullptr) {
            callback(value);
        }
    }

  public:
    ~BaseWidgetValue() override = default;
};

#endif // BASE_WIDGET_VALUE_H