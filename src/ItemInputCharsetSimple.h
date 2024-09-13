#ifndef ItemInputCharsetSimple_H
#define ItemInputCharset_H

#include <utils/utils.h>

#include "ItemInput.h"

class ItemInputCharsetSimple : public ItemInput {
   private:
    const char* charset;
    const uint8_t charsetSize;
    // Active index of the charset
    int8_t charsetPosition = -1;
    bool charEditMode = false;

    /**
     * @brief Initialize `char edit mode`.
     * Set `charEditMode` flag and search for currently selected char in charset.
     */
    void initCharsetEditMode() {
        charEditMode = true;
        if (cursor < strlen(value)) {
            char *e = strchr(charset, value[cursor]);
            if (e != NULL) {
                charsetPosition = (int)(e - charset);
                return;
            }
        }
        charsetPosition = -1;
    }

    /**
     * @brief Stop `char edit mode`.
     * Unset `charEditMode` flag and draw actual char from `value`.
     */
    void stopCharsetEditMode() {
        charEditMode = false;
        if (cursor < strlen(value)) {
            display->drawChar(value[cursor]);
        } else {
            display->drawChar(' ');
        }
    }

   public:
    ItemInputCharsetSimple(const char* text, char* value, const char* charset,
                     const uint8_t charsetSize, fptrStr callback)
        : ItemInput(text, value, callback),
          charset(charset),
          charsetSize(charsetSize) {}

    void enter() override {
        if (!display->getEditModeEnabled()) {
            ItemInput::enter();
            return;
        }
        if (!charEditMode) {
            initCharsetEditMode();
            return;
        }
        uint8_t length = strlen(value);
        if (cursor < length) {
            value[cursor] = charset[charsetPosition];
        } else {
            char* buf = new char[length + 2];
            concat(value, charset[charsetPosition], buf);
            value = buf;
        }
        printCmd(F("CHARSET"), value);
        stopCharsetEditMode();
        ItemInput::right();
    }

    void back() override {
        if (!display->getEditModeEnabled()) {
            return;
        }
        if (!charEditMode) {
            ItemInput::back();
            return;
        }
        stopCharsetEditMode();
    };

    void left() override {
        if (!display->getEditModeEnabled()) {
            return;
        }
        if (!charEditMode) {
            ItemInput::left();
            return;
        }
        charsetPosition = constrain(charsetPosition - 1, 0, charsetSize);
        display->drawChar(charset[charsetPosition]);
    }

    void right() override {
        if (!display->getEditModeEnabled()) {
            return;
        }
        if (!charEditMode) {
            ItemInput::right();
            return;
        }
        charsetPosition = (charsetPosition + 1) % charsetSize;
        display->drawChar(charset[charsetPosition]);
    }

    void up() override {
        // Do nothing
    }

    void down() override {
        // Do nothing
    }

    void clear() override {
        if (!display->getEditModeEnabled()) {
            return;
        }
        ItemInput::clear();
    }

    void typeChar(const char character) override {
        // Do nothing
    }

};

#define ITEM_INPUT_CHARSET_SIMPLE(...) (new ItemInputCharsetSimple(__VA_ARGS__))

#endif
