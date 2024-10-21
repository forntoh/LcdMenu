#ifndef WIDGET_VALUE_H
#define WIDGET_VALUE_H

#include "BaseWidgetValue.h"

template <typename T = void>
class Widget : public BaseWidgetValue<T> {

  protected:
    const T step;

  public:
    Widget(
        T value,
        const T step,
        const char* format,
        const uint8_t cursorOffset = 0,
        void (*callback)(T) = nullptr)
        : BaseWidgetValue<T>(value, format, cursorOffset, callback), step(step) {}

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
                    if (increment()) {
                        BaseWidgetValue<T>::handleChange();
                    }
                    return true;
                case DOWN:
                    if (decrement()) {
                        BaseWidgetValue<T>::handleChange();
                    }
                    return true;
                default:
                    return false;
            }
        }
        return false;
    }
    /**
     * @brief Increment the value.
     */
    virtual bool increment() {
        this->value += step;
        printLog(F("Widget::increment"), this->value);
        return true;
    }
    /**
     * @brief Decrement the value.
     */
    virtual bool decrement() {
        this->value -= step;
        printLog(F("Widget::decrement"), this->value);
        return true;
    }
};

#endif