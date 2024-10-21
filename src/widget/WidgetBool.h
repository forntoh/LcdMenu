// Creator: @ShishkinDmitriy
#ifndef WIDGET_BOOL_H
#define WIDGET_BOOL_H

#include "Widget.h"
#include <utils/utils.h>

/**
 * @brief Widget that allows user to toggle between ON/OFF states.
 * Widget has ON/OFF `enabled` state.
 */
template <>
class Widget<bool> : public BaseWidgetValue<bool> {

  protected:
    const char* textOn = nullptr;
    const char* textOff = nullptr;

  public:
    explicit Widget(
        const boolean value = false,
        const char* textOn = "ON",
        const char* textOff = "OFF",
        const char* format = "%s",
        const uint8_t cursorOffset = 0,
        void (*callback)(bool) = nullptr)
        : BaseWidgetValue(value, format, cursorOffset, callback), textOn(textOn), textOff(textOff) {}

    const char* getTextOn() const { return this->textOn; }

    const char* getTextOff() const { return this->textOff; }

  protected:
    uint8_t draw(char* buffer, const uint8_t size) override {
        return snprintf(buffer, size, format, value ? textOn : textOff);
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
 * @brief Macro to create a new Widget<bool> object.
 *
 * @param value The initial enabled value (default is false)
 * @param textOn The display text when value is true (default is "ON")
 * @param textOff The display text when value is false (default is "OFF")
 * @param format The format to display the value (default is "%s")
 * @param cursorOffset The offset for the cursor (default is 0)
 * @param callback The callback function to execute when value changes (default is nullptr)
 */
#define WIDGET_BOOL(...) (new Widget<bool>(__VA_ARGS__))

#endif