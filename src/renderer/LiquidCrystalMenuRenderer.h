#pragma once

#include "MenuRenderer.h"
#include "display/CharacterDisplayInterface.h"

class CharacterDisplayRenderer : public MenuRenderer {
  protected:
    uint8_t* upArrow;
    uint8_t* downArrow;
    uint8_t cursorIcon;
    uint8_t editCursorIcon;

  public:
    CharacterDisplayRenderer(
        CharacterDisplayInterface* display,
        uint8_t maxCols,
        uint8_t maxRows,
        uint8_t* upArrow = new uint8_t[8]{0x04, 0x0E, 0x1F, 0x04, 0x04, 0x04, 0x04, 0x04},
        uint8_t* downArrow = new uint8_t[8]{0x04, 0x04, 0x04, 0x04, 0x04, 0x1F, 0x0E, 0x04},
        uint8_t cursorIcon = 0x7E,
        uint8_t editCursorIcon = 0x7F)
        : MenuRenderer(display, maxCols, maxRows), upArrow(upArrow), downArrow(downArrow), cursorIcon(cursorIcon), editCursorIcon(editCursorIcon) {}

    void begin() override {
        display->begin();
        static_cast<CharacterDisplayInterface*>(display)->createChar(0, upArrow);
    }

    void drawItem(uint8_t itemIndex, uint8_t screenRow, const char* text) override {
        MenuRenderer::drawItem(itemIndex, screenRow, text);
        // Truncate text if it's too long
        static char* buf = new char[maxCols];
        substring(text, 0, maxCols - (cursorIcon != 0 ? 2 : 1), buf);
        display->setCursor(cursorIcon != 0 ? 1 : 0, cursorRow);
        display->draw(buf);

        uint8_t spaces = constrain(maxCols - (cursorIcon != 0 ? 2 : 1) - strlen(text), 0, maxCols);
        for (uint8_t i = 0; i < spaces; i++) {
            display->draw(" ");
        }
        if (itemIndex == 0) {
            clearUpIndicator();
        } else if (itemIndex == itemCount - 1) {
            clearDownIndicator();
        }

        if (screenRow == 0 && itemIndex > 0) {
            drawUpIndicator();
        } else if (screenRow == maxRows - 1 && itemIndex < itemCount - 1) {
            drawDownIndicator();
        }
    }

    void draw(uint8_t byte) override {
        display->draw(byte);
    }

    void drawCursor() override {
        if (cursorIcon != 0) {
            display->setCursor(cursorCol, cursorRow);
            display->draw(inEditMode ? editCursorIcon : cursorIcon);
        }
    }

    void drawBlinker() override {
        static_cast<CharacterDisplayInterface*>(display)->drawBlinker();
    }

    void clearBlinker() override {
        static_cast<CharacterDisplayInterface*>(display)->clearBlinker();
    }

    void moveCursor(uint8_t cursorCol, uint8_t cursorRow) override {
        MenuRenderer::moveCursor(cursorCol, cursorRow);
        display->setCursor(cursorCol, cursorRow);
    }

    void clearCursor() override {
        if (cursorIcon != 0) {
            display->setCursor(cursorCol, cursorRow);
            display->draw(" ");
        }
    }

    void drawUpIndicator() {
        if (upArrow != NULL) {
            display->setCursor(maxCols - 1, 0);
            display->draw(byte(0));
        }
    }

    void clearUpIndicator() {
        if (upArrow != NULL) {
            display->setCursor(maxCols - 1, 0);
            display->draw(" ");
        }
    }

    void drawDownIndicator() {
        if (downArrow != NULL) {
            display->setCursor(maxCols - 1, maxRows - 1);
            display->draw(byte((1)));
        }
    }

    void clearDownIndicator() {
        if (downArrow != NULL) {
            display->setCursor(maxCols - 1, maxRows - 1);
            display->draw(" ");
        }
    }
};
