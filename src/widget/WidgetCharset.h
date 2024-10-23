// Creator: @ShishkinDmitriy
#pragma once

#include "BaseWidgetValue.h"

/**
 * @class WidgetCharset
 * @brief Widget that allows user to select a character from a charset.
 * Manages a character value within a specified charset, allowing cycling through characters.
 */
class WidgetCharset : public BaseWidgetValue<char> {
    friend class ItemCharsetWidgets;

  protected:
    const char*& charset;
    const bool cycle;
    uint8_t charsetPosition = 0;

  public:
    WidgetCharset(
        const char value,
        const char*& charset,
        const char* format,
        const uint8_t cursorOffset,
        const bool cycle,
        void (*callback)(char))
        : BaseWidgetValue<char>(value, format, cursorOffset, callback), charset(charset), cycle(cycle) {}

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
                        value = charset[charsetPosition];
                        BaseWidgetValue<char>::handleChange();
                        printLog(F("WidgetCharset::nextChar"), value);
                    }
                    return true;
                case DOWN:
                    if (previousChar()) {
                        value = charset[charsetPosition];
                        BaseWidgetValue<char>::handleChange();
                        printLog(F("WidgetCharset::previousChar"), value);
                    }
                    return true;
                case BACKSPACE:
                    if (backspace()) {
                        printLog(F("WidgetCharset::backspace"));
                        BaseWidgetValue<char>::handleChange();
                    }
                    return true;
                default:
                    return false;
            }
        }
        return false;
    }

    bool backspace() {
        if (value != '\0') {
            value = '\0';
            charsetPosition = 0;
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