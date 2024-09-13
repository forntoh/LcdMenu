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

    void changeChar(const char character) {
        uint8_t length = strlen(value);
        if (cursor < length) {
            value[cursor] = character;
        } else {
            char* buf = new char[length + 2];
            concat(value, character, buf);
            value = buf;
        }
        MenuItem::draw();
        display->resetBlinker(constrainBlinkerPosition(display->getBlinkerPosition()));
        // Log
        printCmd(F("TYPE-CHARSET"), character);
    }

   public:
    ItemInputCharset(const char* text, char* value, const char* charset,
                     const uint8_t charsetSize, fptrStr callback)
        : ItemInput(text, value, callback),
          charset(charset),
          charsetSize(charsetSize) {}
    void up() override {
        if (!display->getEditModeEnabled()) {
            return;
        }
        charsetPosition = (charsetPosition + 1) % charsetSize;
        changeChar(charset[charsetPosition]);
    }

    void down() override {
        if (!display->getEditModeEnabled()) {
            return;
        }
        charsetPosition = (charsetPosition - 1) % charsetSize;
        changeChar(charset[charsetPosition]);
    }

    void clear() override {
        MenuItem::clear();
        charsetPosition = -1;
    }
};

#endif
