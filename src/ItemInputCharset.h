#ifndef ItemInputCharset_H
#define ItemInputCharset_H

#include <utils/utils.h>

#include "ItemInput.h"

class ItemInputCharset : public ItemInput {
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
    ItemInputCharset(const char* text, char* value, const char* charset, fptrStr callback)
        : ItemInput(text, value, callback), charset(charset), charsetSize(strlen(charset)) {}
    ItemInputCharset(const char* text, const char* charset, fptrStr callback)
        : ItemInputCharset(text, (char*)"", charset, callback) {}

    void enter() override {
        if (!display->getEditModeEnabled()) {
            ItemInput::enter();
            return;
        }
        if (!charEditMode) {
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
        if (charEditMode) {
            stopCharsetEditMode();
        }
        ItemInput::left();
    }

    void right() override {
        if (!display->getEditModeEnabled()) {
            return;
        }
        if (charEditMode) {
            stopCharsetEditMode();
        }
        ItemInput::right();
    }

    void down() override {
        if (!display->getEditModeEnabled()) {
            return;
        }
        if (!charEditMode) {
            initCharsetEditMode();
        }
        charsetPosition = (charsetPosition + 1) % charsetSize;
        display->drawChar(charset[charsetPosition]);
    }

    void up() override {
        if (!display->getEditModeEnabled()) {
            return;
        }
        if (!charEditMode) {
            initCharsetEditMode();
        }
        charsetPosition = constrain(charsetPosition - 1, 0, charsetSize);
        display->drawChar(charset[charsetPosition]);
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

#define ITEM_INPUT_CHARSET(...) (new ItemInputCharset(__VA_ARGS__))

#endif
