#pragma once

#include "BaseWidgetValue.h"

/**
 * @class WidgetCharset
 * @brief Widget that allows a user to select a character from a charset.
 * Manages a character value within a specified charset, allowing cycling through characters.
 */
class WidgetCharset : public BaseWidgetValue<char> {

  protected:
    const char*& charset;
    const bool cycle;
    int8_t charsetPosition = 0;

  public:
    WidgetCharset(
        int8_t charsetPosition,
        const char*& charset,
        const char* format,
        const uint8_t cursorOffset,
        const bool cycle,
        void (*callback)(const char&))
        : BaseWidgetValue<char>(
              charsetPosition == -1 ? '\0' : charset[charsetPosition],
              format,
              cursorOffset,
              callback),
          charset(charset), cycle(cycle) {}

  protected:
    uint8_t draw(char* buffer, const uint8_t start) override {
        if (start >= ITEM_DRAW_BUFFER_SIZE) return 0;
        return snprintf(buffer + start, ITEM_DRAW_BUFFER_SIZE - start, format, value == '\0' ? ' ' : value);
    }

    /**
     * @brief Process command.
     *
     * Handle commands:
     * - `UP` - go to the next character in the charset and trigger callback
     * - `DOWN` - go to the previous character in the charset and trigger callback
     * - `BACKSPACE` - clear the value
     */
    bool process(LcdMenu* menu, const unsigned char command) override {
        MenuRenderer* renderer = menu->getRenderer();
        if (renderer->isInEditMode()) {
            switch (command) {
                case UP:
                    if (nextChar()) {
                        updateValue(F("WidgetCharset::nextChar"));
                    }
                    return true;
                case DOWN:
                    if (previousChar()) {
                        updateValue(F("WidgetCharset::previousChar"));
                    }
                    return true;
                case BACKSPACE:
                    if (backspace()) {
                        updateValue(F("WidgetCharset::backspace"));
                    }
                    return true;
                default:
                    return false;
            }
        }
        return false;
    }

    void updateValue(const __FlashStringHelper* action) {
        value = charset[charsetPosition];
        BaseWidgetValue<char>::handleChange();
        printLog(action, value);
    }

    bool backspace() {
        if (value != '\0') {
            value = '\0';
            charsetPosition = -1;
            return true;
        }
        return false;
    }

    bool nextChar() {
        if (charset[charsetPosition + 1] != '\0') {
            charsetPosition++;
            return true;
        }
        if (cycle) {
            charsetPosition = 0;
            return true;
        }
        return false;
    }

    bool previousChar() {
        if (charsetPosition > 0) {
            charsetPosition--;
            return true;
        }
        if (cycle) {
            charsetPosition = strlen(charset) - 1;
            return true;
        }
        return false;
    }
};

/**
 * @brief Function to create a new WidgetCharset object.
 *
 * @param charset The charset to use for the widget.
 * @param charsetPosition The initial position in the charset (default is -1 for no initial value).
 * @param format The format to display the value (default is "%c").
 * @param cursorOffset The offset for the cursor (default is 0).
 * @param cycle Whether the value should cycle when out of range (default is false).
 * @param callback The callback function to execute when value changes (default is nullptr).
 */
inline BaseWidgetValue<char>* WIDGET_CHARSET(
    const char*& charset,
    int8_t charsetPosition = -1,
    const char* format = "%c",
    uint8_t cursorOffset = 0,
    bool cycle = false,
    void (*callback)(const char&) = nullptr) {
    return new WidgetCharset(charsetPosition, charset, format, cursorOffset, cycle, callback);
}