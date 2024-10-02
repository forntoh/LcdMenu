#ifndef Control_H
#define Control_H

#include "LcdMenu.h"
#include "MenuItem.h"
#include "printf.h"
#include <utils/utils.h>
#include <limits.h>

template <typename T>
class Control {
    template <typename T0, typename T1, typename T2>
    friend class ItemControl;

  protected:
    T value;
    virtual bool process(LcdMenu* menu, const unsigned char command) = 0;
    virtual uint8_t draw(DisplayInterface* display, uint8_t row, uint8_t col) = 0;
    virtual uint8_t getBlinkerOffset() = 0;

  public:
    Control(T value) : value(value) {}
    T getValue() {
        return value;
    }
    virtual void setValue(T value) {
        this->value = value;
    }
};

// template <typename T>
// struct NumberControlConfig {
//     T min;
//     T max;
//     T step;
//     const char* format;
//     void (*callback)(T) = NULL;
//     const bool cycle = false;
//     const uint8_t blinkerOffset = 0;
// };

template <typename T>
class NumberControl : public Control<T> {
  protected:
    const T minValue;
    const T maxValue;
    const T step;
    const char* format = "%d";
    void (*callback)(T) = NULL;
    const bool cycle = false;
    const uint8_t blinkerOffset = 0;

  public:
    NumberControl(
        const T min,
        const T max,
        T value,
        const T step,
        const char* format,
        bool cycle = false,
        const uint8_t blinkerOffset = 0,
        void (*callback)(T) = NULL)
        : Control<T>(value),
          minValue(min),
          maxValue(max),
          step(step), format(format),
          callback(callback),
          cycle(cycle),
          blinkerOffset(blinkerOffset) {}
    // NumberControl(struct NumberControlConfig<T>& config)
    //     : Control<T>(config.initial),
    //       minValue(config.min),
    //       maxValue(config.max),
    //       step(config.step),
    //       format(config.format),
    //       callback(config.callback),
    //       cycle(config.cycle),
    //       blinkerOffset(config.blinkerOffset) {}

    /**
     * @brief Sets the value.
     *
     * @param value The value to set.
     * @note You need to call `LcdMenu::refresh` after this method to see the changes.
     * @return true if the value was set successfully, false otherwise.
     */
    void setValue(T value) override {
        if (value < minValue || value > maxValue || value == value) {
            return;
        }
        this->value = value;
    }

  protected:
    /**
     * @brief Increments the value.
     */
    bool increment() {
        if (this->value + step > maxValue) {
            if (cycle) {
                this->value = minValue;
                return true;
            }
            return false;
        }
        this->value += step;
        printLog(F("NumberControl::increment"), this->value);
        return true;
    }

    /**
     * @brief Decrements the value.
     */
    bool decrement() {
        if (this->value - step < minValue) {
            if (cycle) {
                this->value = maxValue;
                return true;
            }
            return false;
        }
        this->value -= step;
        printLog(F("NumberControl::decrement"), this->value);
        return true;
    }

    bool process(LcdMenu* menu, const unsigned char command) override {
        DisplayInterface* display = menu->getDisplay();
        if (display->getEditModeEnabled()) {
            switch (command) {
                case UP:
                    if (increment()) {
                        if (callback != NULL) {
                            callback(this->value);
                        }
                    }
                    return true;
                case DOWN:
                    if (decrement()) {
                        if (callback != NULL) {
                            callback(this->value);
                        }
                    }
                    return true;
                default:
                    return false;
            }
        } else {
            return false;
        }
    }
    uint8_t getBlinkerOffset() override {
        return blinkerOffset;
    };

    uint8_t draw(DisplayInterface* display, uint8_t row, uint8_t col) override {
        uint8_t maxCols = display->getMaxCols();
        if (col > maxCols) {
            return -1;
        }
        static char buffer[20];
        int size = sprintf_(buffer, format, this->value);
        if (col + size > maxCols) {
            substring(buffer, 0, maxCols - col, buffer);
        }
        display->drawAt(row, col, buffer);
        return size;
    }
};

#endif  // Control_H
