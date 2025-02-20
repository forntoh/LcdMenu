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
     * @brief Constructor that takes a pointer to an external value.
     * @param value The initial value to set.
     * @param textOn The display text when value is true.
     * @param textOff The display text when value is false.
     * @param format The format string for displaying the value.
     * @param cursorOffset The cursor offset for the widget.
     * @param callback The callback function to call when the value changes.
     */
    WidgetBool(
        bool* value,
        const char* textOn,
        const char* textOff,
        const char* format,
        const uint8_t cursorOffset,
        void (*callback)(bool))
        : BaseWidgetValue<bool>(value, format, cursorOffset, callback), textOn(textOn), textOff(textOff) {}

    const char* getTextOn() const { return this->textOn; }

    const char* getTextOff() const { return this->textOff; }

  protected:
    uint8_t draw(char* buffer, const uint8_t start) override {
        if (start >= ITEM_DRAW_BUFFER_SIZE || valuePtr == nullptr) return 0;
        return snprintf(buffer + start, ITEM_DRAW_BUFFER_SIZE - start, format, *valuePtr ? textOn : textOff);
    }
    /**
     * @brief Process command.
     *
     * Handle commands:
     * - `ENTER` - toggle value and trigger callback;
     *
     * @return true if command was handled, false otherwise
     */
    bool process(LcdMenu* menu, const unsigned char command) override {
        MenuRenderer* renderer = menu->getRenderer();
        if (renderer->isInEditMode()) {
            if (command == UP || command == DOWN) {
                *valuePtr = !*valuePtr;
                LOG(F("WidgetToggle::toggle"), *valuePtr);
                handleChange();
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
inline BaseWidgetValue<bool>* WIDGET_BOOL_REF(
    bool& value,
    const char* textOn = "ON",
    const char* textOff = "OFF",
    const char* format = "%s",
    const uint8_t cursorOffset = 0,
    void (*callback)(bool) = nullptr) {
    return new WidgetBool(&value, textOn, textOff, format, cursorOffset, callback);
}