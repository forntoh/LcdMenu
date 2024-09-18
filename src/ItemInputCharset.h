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
    void stopCharsetEditMode(DisplayInterface* display) {
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

    bool process(Context context) override {
        const unsigned char c = context.command;
        switch (c) {
            case ENTER: return enter(context);
            case BACK: return back(context);
            case UP: return up(context);
            case DOWN: return down(context);
            case LEFT: return left(context);
            case RIGHT: return right(context);
            case BACKSPACE: return ItemInput::backspace(context);
            case CLEAR: return ItemInput::clear(context);
            default: return false;
        }
    }

  protected:
    bool enter(Context context) {
        DisplayInterface* display = context.display;
        if (!display->getEditModeEnabled()) {
            return ItemInput::enter(context);
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
        stopCharsetEditMode(display);
        ItemInput::right(context);
        return true;
    }

    bool back(Context context) {
        DisplayInterface* display = context.display;
        if (!display->getEditModeEnabled()) {
            return false;
        }
        if (!charEditMode) {
            return ItemInput::back(context);
        }
        stopCharsetEditMode(display);
        return true;
    };

    bool left(Context context) {
        DisplayInterface* display = context.display;
        if (!display->getEditModeEnabled()) {
            return false;
        }
        if (charEditMode) {
            stopCharsetEditMode(display);
        }
        return ItemInput::left(context);
    }

    bool right(Context context) {
        DisplayInterface* display = context.display;
        if (!display->getEditModeEnabled()) {
            return false;
        }
        if (charEditMode) {
            stopCharsetEditMode(display);
        }
        return ItemInput::right(context);
    }

    bool down(Context context) {
        DisplayInterface* display = context.display;
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

    bool up(Context context) {
        DisplayInterface* display = context.display;
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
