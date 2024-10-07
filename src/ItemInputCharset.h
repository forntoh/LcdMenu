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
     * @param text The text to renderer for the item.
     * @param value The initial value for the input.
     * @param charset The charset to use for input.
     * @param callback A reference to the callback function to be invoked when
     * the input is submitted.
     */
    ItemInputCharset(const char* text, char* value, const char* charset, fptrStr callback)
        : ItemInput(text, value, callback), charset(charset) {}

    /**
     * @brief Construct a new ItemInputCharset object with no initial value.
     * @param text The text to renderer for the item.
     * @param charset The charset to use for input.
     * @param callback A reference to the callback function to be invoked when
     * the input is submitted.
     */
    ItemInputCharset(const char* text, const char* charset, fptrStr callback)
        : ItemInputCharset(text, (char*)"", charset, callback) {}

  protected:
    bool process(LcdMenu* menu, const unsigned char command) override {
        MenuRenderer* renderer = menu->getRenderer();
        if (renderer->isInEditMode()) {
            switch (command) {
                case ENTER:
                    if (charEdit) {
                        commitCharEdit(renderer);
                    }
                    return true;
                case BACK:
                    if (charEdit) {
                        abortCharEdit(renderer);
                    } else {
                        ItemInput::back(renderer);
                    }
                    return true;
                case UP:
                    if (!charEdit) {
                        initCharEdit();
                    }
                    showPreviousChar(renderer);
                    printLog(F("ItemInputCharset::up"), charset[charsetPosition]);
                    return true;
                case DOWN:
                    if (!charEdit) {
                        initCharEdit();
                    }
                    showNextChar(renderer);
                    printLog(F("ItemInputCharset::down"), charset[charsetPosition]);
                    return true;
                case LEFT:
                    if (charEdit) {
                        abortCharEdit(renderer);
                    }
                    ItemInput::left(renderer);
                    return true;
                case RIGHT:
                    if (charEdit) {
                        abortCharEdit(renderer);
                    }
                    ItemInput::right(renderer);
                    return true;
                case BACKSPACE:
                    ItemInput::backspace(renderer);
                    return true;
                case CLEAR:
                    ItemInput::clear(renderer);
                    return true;
                default:
                    return false;
            }
        } else {
            switch (command) {
                case ENTER:
                    ItemInput::enter(renderer);
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
    void abortCharEdit(MenuRenderer* renderer) {
        charEdit = false;
        if (cursor < strlen(value)) {
            renderer->draw(value[cursor]);
        } else {
            renderer->draw(' ');
        }
        printLog(F("ItemInputCharset::abortCharEdit"));
    }
    /**
     * @brief Commit char edit mode.
     * Replace char at cursor position with selected char from charset.
     */
    void commitCharEdit(MenuRenderer* renderer) {
        uint8_t length = strlen(value);
        if (cursor < length) {
            value[cursor] = charset[charsetPosition];
        } else {
            char* buf = new char[length + 2];
            concat(value, charset[charsetPosition], buf);
            value = buf;
        }
        abortCharEdit(renderer);
        printLog(F("ItemInputCharset::commitCharEdit"), charset[charsetPosition]);
        ItemInput::right(renderer);
    }
    /**
     * @brief Show next char from charset.
     */
    void showNextChar(MenuRenderer* renderer) {
        if (charset[charsetPosition + 1] == '\0') {
            charsetPosition = 0;
        } else {
            charsetPosition++;
        }
        drawChar(renderer);
    }
    /**
     * @brief Show previous char from charset.
     */
    void showPreviousChar(MenuRenderer* renderer) {
        if (charsetPosition == 0) {
            charsetPosition = strlen(charset) - 1;
        } else {
            charsetPosition--;
        }
        drawChar(renderer);
    }

    void drawChar(MenuRenderer* renderer) {
        renderer->moveCursor(renderer->getCursorCol(), renderer->getCursorRow());
        renderer->draw(charset[charsetPosition]);
        renderer->moveCursor(renderer->getCursorCol(), renderer->getCursorRow());
    }
};

#define ITEM_INPUT_CHARSET(...) (new ItemInputCharset(__VA_ARGS__))

#endif
