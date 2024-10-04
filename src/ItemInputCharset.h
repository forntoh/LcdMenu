#ifndef ItemInputCharset_H
#define ItemInputCharset_H

#include "ItemInput.h"
#include "LcdMenu.h"
#include <utils/utils.h>

class ItemInputCharset : public ItemInput {
  private:
    const char* charset;
    // Active index of the charset
    int8_t charsetPosition = -1;
    bool charEdit = false;

  public:
    /**
     * @brief Construct a new ItemInputCharset object with an initial value.
     *
     * @param text The text to display for the item.
     * @param value The initial value for the input.
     * @param charset The charset to use for input.
     * @param callback A reference to the callback function to be invoked when
     * the input is submitted.
     */
    ItemInputCharset(const char* text, char* value, const char* charset, fptrStr callback)
        : ItemInput(text, value, callback), charset(charset) {}

    /**
     * @brief Construct a new ItemInputCharset object with no initial value.
     * @param text The text to display for the item.
     * @param charset The charset to use for input.
     * @param callback A reference to the callback function to be invoked when
     * the input is submitted.
     */
    ItemInputCharset(const char* text, const char* charset, fptrStr callback)
        : ItemInputCharset(text, (char*)"", charset, callback) {}

  protected:
    bool process(LcdMenu* menu, const unsigned char command) override {
        DisplayInterface* display = menu->getDisplay();
        if (display->getEditModeEnabled()) {
            switch (command) {
                case ENTER:
                    if (charEdit) {
                        commitCharEdit(display);
                    }
                    return true;
                case BACK:
                    if (charEdit) {
                        abortCharEdit(display);
                    } else {
                        ItemInput::back(display);
                    }
                    return true;
                case UP:
                    if (!charEdit) {
                        initCharEdit();
                    }
                    showPreviousChar(display);
                    printLog(F("ItemInputCharset::up"), charset[charsetPosition]);
                    return true;
                case DOWN:
                    if (!charEdit) {
                        initCharEdit();
                    }
                    showNextChar(display);
                    printLog(F("ItemInputCharset::down"), charset[charsetPosition]);
                    return true;
                case LEFT:
                    if (charEdit) {
                        abortCharEdit(display);
                    }
                    ItemInput::left(display);
                    return true;
                case RIGHT:
                    if (charEdit) {
                        abortCharEdit(display);
                    }
                    ItemInput::right(display);
                    return true;
                case BACKSPACE:
                    ItemInput::backspace(display);
                    return true;
                case CLEAR:
                    ItemInput::clear(display);
                    return true;
                default:
                    return false;
            }
        } else {
            switch (command) {
                case ENTER:
                    ItemInput::enter(display);
                    return true;
                default:
                    return false;
            }
        }
    }
    /**
     * @brief Initialize `char edit mode`.
     * Set `charEdit` flag and search for currently selected char in charset.
     */
    void initCharEdit() {
        charEdit = true;
        if (cursor < strlen(value)) {
            char* e = strchr(charset, value[cursor]);
            if (e != NULL) {
                charsetPosition = (int)(e - charset);
                return;
            }
        }
        charsetPosition = -1;
    }
    /**
     * @brief Stop `char edit mode` and abort all mde changes.
     * Unset `charEdit` flag and draw actual char from `value`.
     */
    void abortCharEdit(DisplayInterface* display) {
        charEdit = false;
        if (cursor < strlen(value)) {
            display->drawChar(value[cursor]);
        } else {
            display->drawChar(' ');
        }
        printLog(F("ItemInputCharset::abortCharEdit"));
    }
    /**
     * @brief Commit char edit mode.
     * Replace char at cursor position with selected char from charset.
     */
    void commitCharEdit(DisplayInterface* display) {
        uint8_t length = strlen(value);
        if (cursor < length) {
            value[cursor] = charset[charsetPosition];
        } else {
            char* buf = new char[length + 2];
            concat(value, charset[charsetPosition], buf);
            value = buf;
        }
        abortCharEdit(display);
        printLog(F("ItemInputCharset::commitCharEdit"), charset[charsetPosition]);
        ItemInput::right(display);
    }
    /**
     * @brief Show next char from charset.
     */
    void showNextChar(DisplayInterface* display) {
        if (charset[charsetPosition + 1] == '\0') {
            charsetPosition = 0;
        } else {
            charsetPosition++;
        }
        display->drawChar(charset[charsetPosition]);
    }
    /**
     * @brief Show previous char from charset.
     */
    void showPreviousChar(DisplayInterface* display) {
        charsetPosition = max(charsetPosition - 1, 0);
        display->drawChar(charset[charsetPosition]);
    }
};

#define ITEM_INPUT_CHARSET(...) (new ItemInputCharset(__VA_ARGS__))

#endif
