#ifndef Control_H
#define Control_H

#include "LcdMenu.h"
#include "MenuItem.h"
#include "printf.h"
#include <utils/utils.h>

#define WIDGET_BUFFER_SIZE 20

class Control {
    template <typename T0, typename T1, typename T2>
    friend class ItemControl;
    friend class ItemControlBaseMany;

  protected:
    virtual bool process(LcdMenu* menu, const unsigned char command) = 0;
    virtual uint8_t draw(DisplayInterface* display, uint8_t row, uint8_t col) = 0;
    virtual int draw2(char* buffer, size_t size) = 0;
    virtual uint8_t getBlinkerOffset() = 0;
};

template <typename T>
class ControlValue : public Control {
  protected:
    T value;

  public:
    ControlValue(T value) : Control(), value(value) {}
    T getValue() {
        return value;
    }
    virtual void setValue(T value) {
        this->value = value;
    }
};

template <typename T>
class ControlValueLimited : public ControlValue<T> {
  protected:
    const T minValue;
    const T maxValue;
    const T step;
    const char* format = "%d";
    void (*callback)(T) = NULL;
    const bool cycle = false;
    const uint8_t blinkerOffset = 0;

  public:
    ControlValueLimited(
        const T min,
        const T max,
        T value,
        const T step,
        const char* format,
        bool cycle = false,
        const uint8_t blinkerOffset = 0,
        void (*callback)(T) = NULL)
        : ControlValue<T>(value),
          minValue(min),
          maxValue(max),
          step(step), format(format),
          callback(callback),
          cycle(cycle),
          blinkerOffset(blinkerOffset) {}

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

    int draw2(char* buffer, size_t size) override {
        return snprintf_(buffer, size, format, this->value);
        // Serial.print("Draw2 (");
        // Serial.print(format);
        // Serial.print(", ");
        // Serial.print(this->value);
        // Serial.print(") ");
        // Serial.println(buffer);
    }

    uint8_t draw(DisplayInterface* display, uint8_t row, uint8_t col) override {
        uint8_t maxCols = display->getMaxCols();
        if (col > maxCols) {
            return -1;
        }
        static char buffer[WIDGET_BUFFER_SIZE];
        int size = sprintf_(buffer, format, this->value);
        if (col + size > maxCols) {
            substring(buffer, 0, maxCols - col, buffer);
        }
        display->drawAt(row, col, buffer);
        return size;
    }
};

#endif  // Control_H
