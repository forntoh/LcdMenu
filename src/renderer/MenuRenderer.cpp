#include "MenuRenderer.h"

MenuRenderer::MenuRenderer(DisplayInterface* display, uint8_t maxCols, uint8_t maxRows)
    : maxCols(maxCols), maxRows(maxRows), display(display) {}

void MenuRenderer::begin() {
    display->begin();
    startTime = millis();
}

void MenuRenderer::drawItem(uint8_t itemIndex, uint8_t screenRow, const char* text) {
    this->cursorRow = screenRow;
    this->itemIndex = itemIndex;
}

void MenuRenderer::moveCursor(uint8_t cursorCol, uint8_t cursorRow) {
    this->cursorCol = cursorCol;
    this->cursorRow = cursorRow;
}

void MenuRenderer::setEditMode(bool inEditMode) {
    if (this->inEditMode != inEditMode) {
        this->inEditMode = inEditMode;
        moveCursor(0, cursorRow);
    }
}

void MenuRenderer::restartTimer() {
    this->startTime = millis();
    display->show();
}

bool MenuRenderer::isInEditMode() const { return inEditMode; }

uint8_t MenuRenderer::getCursorCol() const { return cursorCol; }

uint8_t MenuRenderer::getCursorRow() const { return cursorRow; }

uint8_t MenuRenderer::getMaxRows() const { return maxRows; }

uint8_t MenuRenderer::getMaxCols() const { return maxCols; }

uint8_t MenuRenderer::getItemIndex() const { return itemIndex; }

uint8_t MenuRenderer::getActiveRow() const { return activeRow; }