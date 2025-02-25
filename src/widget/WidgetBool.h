// Creator: @ShishkinDmitriy
#pragma once

#include "BaseWidgetValue.h"

/**
 * @brief Widget that allows user to toggle between ON/OFF states.
 * Manages a boolean value, allowing toggling between 'ON' and 'OFF' states.
 */
class WidgetBool : public BaseWidgetValue<bool> {

  protected:
    const char* textOn;
    const char* textOff;

  public:
    /**
     * @brief Constructor that takes a value and creates a new internal value.
     * @param value The initial value to set.
     * @param textOn The display text when value is true.
     * @param textOff The display text when value is false.
     * @param format The format string for displaying the value.
     * @param cursorOffset The cursor offset for the widget.
     * @param callback The callback function to call when the value changes.
     */
    WidgetBool(
        bool value,
        const char* textOn,
        const char* textOff,
        const char* format,
        const uint8_t cursorOffset,
        void (*callback)(bool))
        : BaseWidgetValue<bool>(value, format, cursorOffset, callback), textOn(textOn), textOff(textOff) {}

    /**
     * @brief Constructor that takes a reference to an external value.
     * @param ref A reference to the value.
     * @param textOn The display text when value is true.
     * @param textOff The display text when value is false.
     * @param format The format string for displaying the value.
     * @param cursorOffset The cursor offset for the widget.
     * @param callback The callback function to call when the value changes.
     */
    WidgetBool(
        Ref<bool> ref,
        const char* textOn,
        const char* textOff,
        const char* format,
        const uint8_t cursorOffset,
        void (*callback)(bool))
        : BaseWidgetValue<bool>(ref, format, cursorOffset, callback), textOn(textOn), textOff(textOff) {}

    /**
     * @brief Toggle the boolean value.
     */
    void toggle() {
        setValue(!getValue());
    }

  protected:
    /**
     * @brief Draw the widget into the specified buffer.
     * @param buffer The buffer where the widget will be drawn.
     * @param start The index where to start drawing in the buffer.
     * @return The number of characters written to the buffer.
     */
    uint8_t draw(char* buffer, const uint8_t start) override {
        return snprintf(buffer + start, ITEM_DRAW_BUFFER_SIZE - start, format, getValue() ? textOn : textOff);
    }
    /**
     * @brief Process command.
     *
     * Handle commands:
     * - `ENTER` - toggle value and trigger callback;
     * @param menu Pointer to the menu instance.
     * @param command The command to process.
     * @return true if command was handled, false otherwise
     */
    bool process(LcdMenu* menu, const unsigned char command) override {
        MenuRenderer* renderer = menu->getRenderer();
        if (renderer->isInEditMode()) {
            if (command == UP || command == DOWN) {
                toggle();
                LOG(F("WidgetToggle::toggle"), getValue());
                return true;
            }
        }
        return false;
    }
};

/**
 * @brief Function to create a new Widget<bool> object.
 *
 * @param value The initial enabled value
 * @param textOn The display text when value is true (default is "ON")
 * @param textOff The display text when value is false (default is "OFF")
 * @param format The format to display the value (default is "%s")
 * @param cursorOffset The offset for the cursor (default is 0)
 * @param callback The callback function to execute when value changes (default is nullptr)
 */
inline BaseWidgetValue<bool>* WIDGET_BOOL(
    bool value,
    const char* textOn = "ON",
    const char* textOff = "OFF",
    const char* format = "%s",
    const uint8_t cursorOffset = 0,
    void (*callback)(bool) = nullptr) {
    return new WidgetBool(value, textOn, textOff, format, cursorOffset, callback);
}

/**
 * @brief Function to create a new Widget<bool> object.
 *
 * @param value The initial enabled value (this value is passed by reference, so it can be updated externally)
 * @param textOn The display text when value is true (default is "ON")
 * @param textOff The display text when value is false (default is "OFF")
 * @param format The format to display the value (default is "%s")
 * @param cursorOffset The offset for the cursor (default is 0)
 * @param callback The callback function to execute when value changes (default is nullptr)
 */
inline BaseWidgetValue<bool>* WIDGET_BOOL(
    Ref<bool> ref,
    const char* textOn = "ON",
    const char* textOff = "OFF",
    const char* format = "%s",
    const uint8_t cursorOffset = 0,
    void (*callback)(bool) = nullptr) {
    return new WidgetBool(ref, textOn, textOff, format, cursorOffset, callback);
}
