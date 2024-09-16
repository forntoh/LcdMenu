#ifndef ItemInputCharset_H
#define ItemInputCharset_H

#include "ItemInput.h"
#include <utils/utils.h>

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
            char* e = strchr(charset, value[cursor]);
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

    bool handle(const char c) override {
        switch (c) {
            case ENTER: return enter();
            case BACK: return back();
            case UP: return up();
            case DOWN: return down();
            case LEFT: return left();
            case RIGHT: return right();
            case BACKSPACE: return ItemInput::backspace();
            case CLEAR: return ItemInput::clear();
            default: return false;
        }
    }

   protected:
    bool enter() {
        if (!display->getEditModeEnabled()) {
            return ItemInput::enter();
        }
        if (!charEditMode) {
            return true;
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
        return true;
    }

    bool back() {
        if (!display->getEditModeEnabled()) {
            return false;
        }
        if (!charEditMode) {
            return ItemInput::back();
        }
        stopCharsetEditMode();
        return true;
    };

    bool left() {
        if (!display->getEditModeEnabled()) {
            return false;
        }
        if (charEditMode) {
            stopCharsetEditMode();
        }
        return ItemInput::left();
    }

    bool right() {
        if (!display->getEditModeEnabled()) {
            return false;
        }
        if (charEditMode) {
            stopCharsetEditMode();
        }
        return ItemInput::right();
    }

    bool down() {
        if (!display->getEditModeEnabled()) {
            return false;
        }
        if (!charEditMode) {
            initCharsetEditMode();
        }
        charsetPosition = (charsetPosition + 1) % charsetSize;
        display->drawChar(charset[charsetPosition]);
        return true;
    }

    bool up() {
        if (!display->getEditModeEnabled()) {
            return false;
        }
        if (!charEditMode) {
            initCharsetEditMode();
        }
        charsetPosition = constrain(charsetPosition - 1, 0, charsetSize);
        display->drawChar(charset[charsetPosition]);
        return true;
    }

};

#define ITEM_INPUT_CHARSET(...) (new ItemInputCharset(__VA_ARGS__))

#endif
