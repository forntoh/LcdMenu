// Creator: @ShishkinDmitriy
#ifndef WIDGET_BOOL_H
#define WIDGET_BOOL_H

#include "BaseWidgetValue.h"

/**
 * @brief Widget that allows user to toggle between ON/OFF states.
 * Manages a boolean value, allowing toggling between 'ON' and 'OFF' states
 */
class WidgetBool : public BaseWidgetValue<bool> {

  protected:
    const char* textOn;
    const char* textOff;

  public:
    WidgetBool(
        const bool value,
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
        if (start >= ITEM_DRAW_BUFFER_SIZE) return 0;
        return snprintf(buffer + start, ITEM_DRAW_BUFFER_SIZE, format, value ? textOn : textOff);
    }
    /**
     * @brief Process command.
     *
     * Handle commands:
     * - `ENTER` - toggle value and trigger callback;
     */
    bool process(LcdMenu* menu, const unsigned char command) override {
        if (command == ENTER) {
            value = !value;
            printLog(F("WidgetToggle::toggle"), value);
            handleChange();
            return true;
        }
        return false;
    }
};

/**
 * @brief Function to create a new Widget<bool> object.
 *
 * @param value The initial enabled value (default is false)
 * @param textOn The display text when value is true (default is "ON")
 * @param textOff The display text when value is false (default is "OFF")
 * @param format The format to display the value (default is "%s")
 * @param cursorOffset The offset for the cursor (default is 0)
 * @param callback The callback function to execute when value changes (default is nullptr)
 */
inline BaseWidgetValue<bool>* WIDGET_BOOL(
    const bool value = false,
    const char* textOn = "ON",
    const char* textOff = "OFF",
    const char* format = "%s",
    const uint8_t cursorOffset = 0,
    void (*callback)(bool) = nullptr) {
    return new WidgetBool(value, textOn, textOff, format, cursorOffset, callback);
}

#endif