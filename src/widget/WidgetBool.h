// Creator: @ShishkinDmitriy
#pragma once

#include "BaseWidgetValue.h"

/**
 * @class WidgetBool
 * @brief Widget that allows user to toggle between ON/OFF states.
 * Manages a boolean value, allowing toggling between 'ON' and 'OFF' states.
 * @tparam V the type of stored value, the type should be fully compatible with `bool` type, meaning
 * all arifmetic operations, cast, assignment should be supported for type `V`. For example, V = Ref<bool>.
 */
template <typename V = bool>
class WidgetBool : public BaseWidgetValue<V> {

  protected:
    const char* textOn;
    const char* textOff;

  public:
    WidgetBool(
        V value,
        const char* textOn,
        const char* textOff,
        const char* format,
        const uint8_t cursorOffset,
        void (*callback)(const V&) = nullptr)
        : BaseWidgetValue<V>(value, format, cursorOffset, callback), textOn(textOn), textOff(textOff) {}

    const char* getTextOn() const { return this->textOn; }

    const char* getTextOff() const { return this->textOff; }

  protected:
    uint8_t draw(char* buffer, const uint8_t start) override {
        if (start >= ITEM_DRAW_BUFFER_SIZE) return 0;
        return snprintf(buffer + start, ITEM_DRAW_BUFFER_SIZE - start, this->format, static_cast<bool>(this->value) ? textOn : textOff);
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
                bool current = static_cast<bool>(this->value);
                this->value = !current;
                LOG(F("WidgetToggle::toggle"), value);
                this->handleChange();
                return true;
            }
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
    const bool value,
    const char* textOn = "ON",
    const char* textOff = "OFF",
    const char* format = "%s",
    const uint8_t cursorOffset = 0,
    void (*callback)(const bool&) = nullptr) {
    return new WidgetBool<bool>(value, textOn, textOff, format, cursorOffset, callback);
}

/**
 * @brief Function to create a new Widget<bool> object.
 *
 * @param value The reference value of this widget, the value will be used by reference
 * @param textOn The display text when value is true (default is "ON")
 * @param textOff The display text when value is false (default is "OFF")
 * @param format The format to display the value (default is "%s")
 * @param cursorOffset The offset for the cursor (default is 0)
 * @param callback The callback function to execute when value changes (default is nullptr), parameter of callback will be `Ref<bool>`
 */
inline BaseWidgetValue<Ref<bool>>* WIDGET_BOOL_REF(
    bool& value,
    const char* textOn = "ON",
    const char* textOff = "OFF",
    const char* format = "%s",
    const uint8_t cursorOffset = 0,
    void (*callback)(const Ref<bool>&) = nullptr) {
    return new WidgetBool<Ref<bool>>(Ref<bool>(value), textOn, textOff, format, cursorOffset, callback);
}
