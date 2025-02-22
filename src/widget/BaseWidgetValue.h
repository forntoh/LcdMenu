// Creator: @ShishkinDmitriy
#pragma once

#include "BaseWidget.h"
#include "data/ValueWrappers.h"

class LcdMenu;

/**
 * @class BaseWidgetValue
 * @brief A template class that represents a widget with a value.
 *
 * This class extends the BaseWidget class and adds functionality to handle a value of type T.
 * It provides methods to get and set the value, draw the widget, and process commands.
 * It also supports a callback function that is invoked when the value changes.
 *
 * @tparam T The type of the value.
 */
template <typename T>
class BaseWidgetValue : public BaseWidget {

  protected:
    T value;                        ///< The value of the widget.
    T* refValue = nullptr;          ///< Pointer to the reference value.
    T* ptrValue = nullptr;          ///< Pointer to the pointer value.
    const char* format = nullptr;   ///< Format string for displaying the value.
    void (*callback)(T) = nullptr;  ///< Callback function to execute when value changes.

  public:
    /**
     * @brief Constructor to initialize the widget with a value.
     *
     * @param value The initial value of the widget.
     * @param format The format string for displaying the value.
     * @param cursorOffset The cursor offset for the widget.
     * @param callback The callback function to execute when the value changes.
     */
    BaseWidgetValue(
        T value,
        const char* format,
        const uint8_t cursorOffset = 0,
        void (*callback)(T) = nullptr)
        : BaseWidget(cursorOffset),
          value(value),
          format(format),
          callback(callback) {}

    /**
     * @brief Constructor to initialize the widget with a reference to a value.
     *
     * @param ref A reference to the value.
     * @param format The format string for displaying the value.
     * @param cursorOffset The cursor offset for the widget.
     * @param callback The callback function to execute when the value changes.
     */
    BaseWidgetValue(
        Ref<T> ref,
        const char* format,
        const uint8_t cursorOffset = 0,
        void (*callback)(T) = nullptr)
        : BaseWidget(cursorOffset),
          refValue(&ref.value),
          format(format),
          callback(callback) {}

    /**
     * @brief Constructor to initialize the widget with a pointer to a value.
     *
     * @param ptr A pointer to the value.
     * @param format The format string for displaying the value.
     * @param cursorOffset The cursor offset for the widget.
     * @param callback The callback function to execute when the value changes.
     */

    BaseWidgetValue(
        Ptr<T> ptr,
        const char* format,
        const uint8_t cursorOffset = 0,
        void (*callback)(T) = nullptr)
        : BaseWidget(cursorOffset),
          ptrValue(ptr.value),
          format(format),
          callback(callback) {}

    const T& getValue() const {
        if (refValue) return *refValue;
        if (ptrValue) return *ptrValue;
        return value;
    }

    /**
     * @brief Set a new value for the widget.
     *
     * If the new value is different from the current value, the value is updated and the handleChange method is called.
     *
     * @param newValue The new value to set.
     */
    virtual void setValue(const T& newValue) {
        if (value != newValue) {
            value = newValue;
            handleChange();
        }
    }

  protected:
    /**
     * @brief Draw the widget into the specified buffer.
     *
     * This method is used to draw the widget's value into a buffer starting at a specified index.
     *
     * @param buffer The buffer where the widget will be drawn.
     * @param start The index where to start drawing in the buffer.
     * @return The number of characters written to the buffer.
     */
    uint8_t draw(char* buffer, const uint8_t start) override {
        if (start >= ITEM_DRAW_BUFFER_SIZE) return 0;
        return snprintf(buffer + start, ITEM_DRAW_BUFFER_SIZE - start, format, getValue());
    }

    /**
     * @brief Process a command for this widget.
     *
     * This is a pure virtual method that must be implemented by derived classes to handle specific commands.
     *
     * @param menu Pointer to the menu instance.
     * @param command The command to process.
     * @return true if the command was handled, false otherwise.
     */
    bool process(LcdMenu* menu, unsigned char command) override = 0;

    /**
     * @brief Handle value change and invoke the callback if set.
     *
     * This method is called when the value of the widget changes. If a callback function is set, it is invoked with the new value.
     */
    void handleChange() {
        if (callback != nullptr) {
            callback(getValue());
        }
    }
};