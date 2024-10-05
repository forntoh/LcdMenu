#ifndef MENU_RENDERER_H
#define MENU_RENDERER_H

#include "display/DisplayInterface.h"
#include <Arduino.h>
#include <utils/utils.h>

class MenuRenderer {
  protected:
    uint8_t maxCols;
    uint8_t maxRows;

    uint8_t cursorCol;
    uint8_t cursorRow;

    uint8_t blinkerPosition;

    uint8_t itemIndex;
    uint8_t itemCount;

    bool inEditMode;

  public:
    DisplayInterface* display;

    MenuRenderer(
        DisplayInterface* display,
        uint8_t maxCols,
        uint8_t maxRows) : maxCols(maxCols), maxRows(maxRows), display(display) {}

    virtual void begin() = 0;

    virtual void draw(uint8_t byte) = 0;

    virtual void drawItem(uint8_t itemIndex, uint8_t screenRow, const char* text) {
        this->cursorRow = screenRow;
        this->itemIndex = itemIndex;
    }

    virtual void drawCursor() = 0;

    virtual void clearBlinker() = 0;

    virtual void drawBlinker() = 0;

    virtual void moveCursor(uint8_t cursorCol, uint8_t cursorRow) {
        this->cursorCol = cursorCol;
        this->cursorRow = cursorRow;
    }

    virtual void clearCursor() = 0;

    void setItemCount(uint8_t itemCount) { this->itemCount = itemCount; }

    void setEditMode(bool inEditMode) {
        if (this->inEditMode != inEditMode) {
            this->inEditMode = inEditMode;
            moveCursor(0, cursorRow);
            clearCursor();
            drawCursor();
        }
    }

    bool isInEditMode() const { return inEditMode; }

    uint8_t getCursorCol() const { return cursorCol; }

    uint8_t getCursorRow() const { return cursorRow; }

    uint8_t getMaxRows() const { return maxRows; }

    uint8_t getMaxCols() const { return maxCols; }

    uint8_t getItemIndex() const { return itemIndex; }
};

#endif  // MENU_RENDERER_H