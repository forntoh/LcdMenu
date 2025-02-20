// Creator: @ShishkinDmitriy
#pragma once

#include "BaseWidget.h"

class LcdMenu;

/**
 * @class BaseWidgetValue
 * @brief Template class for a widget that holds a value.
 *
 * This class provides functionality to manage and display a value within a widget.
 * It supports both owning and non-owning semantics for the value's memory.
 *
 * @tparam T The type of the value held by the widget.
 * @see BaseWidget
 */
template <typename T>
class BaseWidgetValue : public BaseWidget {

  protected:
    T* valuePtr;                    ///< Pointer to the value held by the widget
    const char* format = nullptr;   ///< Format string for displaying the value
    void (*callback)(T) = nullptr;  ///< Callback function to call when the value changes
    bool ownsMemory = false;        ///< Flag indicating whether this instance owns the memory for valuePtr

  public:
    /**
     * @brief Constructor that takes a value and creates a new internal value.
     *
     * This constructor allocates memory for the value and sets the `ownsMemory` flag to true,
     * indicating that this instance exclusively owns the allocated memory.
     *
     * @param value The initial value to set.
     * @param format The format string for displaying the value.
     * @param cursorOffset The cursor offset for the widget.
     * @param callback The callback function to call when the value changes.
     */
    BaseWidgetValue(
        T value,
        const char* format,
        const uint8_t cursorOffset = 0,
        void (*callback)(T) = nullptr)
        : BaseWidget(cursorOffset),
          valuePtr(new T(value)),  ///< Allocate memory for the value
          format(format),
          callback(callback),
          ownsMemory(true) {}  ///< This instance owns the allocated memory

    /**
     * @brief Constructor that takes a pointer to an external value.
     *
     * This constructor uses the provided pointer to an external value and sets the `ownsMemory` flag to false,
     * indicating that this instance does not own the memory for the value.
     *
     * @param ptr Pointer to the external value.
     * @param format The format string for displaying the value.
     * @param cursorOffset The cursor offset for the widget.
     * @param callback The callback function to call when the value changes.
     */
    BaseWidgetValue(
        T* ptr,
        const char* format,
        const uint8_t cursorOffset = 0,
        void (*callback)(T) = nullptr)
        : BaseWidget(cursorOffset),
          valuePtr(ptr),  ///< Use the external pointer
          format(format),
          callback(callback),
          ownsMemory(false) {}  ///< This instance does not own the memory

    /**
     * @brief Destructor that cleans up allocated memory if owned.
     *
     * If this instance owns the memory for the value (i.e., `ownsMemory` is true),
     * the destructor will delete the allocated memory.
     */
    ~BaseWidgetValue() override {
        if (ownsMemory) {
            delete valuePtr;
        }
    }
    /**
     * @brief Retrieve current value.
     *
     * @return The current value held by the widget.
     */
    const T& getValue() const { return *valuePtr; }
    /**
     * @brief Sets the value.
     *
     * If the new value is different from the current value, the value is updated and the callback is invoked.
     *
     * @param newValue The value to set.
     * @note You need to call `LcdMenu::refresh` after this method to see the changes.
     */
    virtual void setValue(const T& newValue) {
        if (*valuePtr != newValue) {
            *valuePtr = newValue;
            handleChange();
        }
    }

  protected:
    /**
     * @brief Draw the widget into specified buffer.
     *
     * @param buffer The buffer where the widget will be drawn.
     * @param start The index where to start drawing in the buffer.
     * @return The number of characters written to the buffer.
     */
    uint8_t draw(char* buffer, const uint8_t start) override {
        if (start >= ITEM_DRAW_BUFFER_SIZE || valuePtr == nullptr) return 0;
        return snprintf(buffer + start, ITEM_DRAW_BUFFER_SIZE - start, format, *valuePtr);
    }

    /**
     * @brief Process a command for this widget.
     *
     * @param menu Pointer to the menu instance.
     * @param command The command to process.
     * @return true if the command was handled, false otherwise.
     */
    bool process(LcdMenu* menu, unsigned char command) override = 0;

    /**
     * @brief Handle value change and invoke the callback if set.
     */
    void handleChange() {
        if (callback != nullptr) {
            callback(*valuePtr);
        }
    }
};