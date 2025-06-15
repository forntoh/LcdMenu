#include "GraphicalDisplayRenderer.h"

GraphicalDisplayRenderer::GraphicalDisplayRenderer(
    GraphicalDisplayInterface* display,
    uint8_t dispWidth,
    uint8_t dispHeight,
    uint8_t chWidth,
    uint8_t chHeight,
    const char* cursorIcon,
    const char* editCursorIcon,
    uint8_t scrollbarWidth)
    : MenuRenderer(display, (dispWidth - scrollbarWidth) / chWidth, dispHeight / chHeight),
      gDisplay(display),
      charWidth(chWidth),
      charHeight(chHeight),
      displayWidth(dispWidth),
      displayHeight(dispHeight),
      cursorIcon(cursorIcon),
      editCursorIcon(editCursorIcon),
      scrollbarWidth(scrollbarWidth) {}

void GraphicalDisplayRenderer::begin() {
    MenuRenderer::begin();
    drawScrollBar();
    gDisplay->sendBuffer();
}

void GraphicalDisplayRenderer::draw(uint8_t byte) {
    char c[2] = {static_cast<char>(byte), '\0'};
    gDisplay->draw(c);
}

void GraphicalDisplayRenderer::drawItem(const char* text, const char* value, bool) {
    uint8_t x = 0;
    uint8_t y = (cursorRow + 1) * charHeight;
    if (cursorIcon != nullptr || editCursorIcon != nullptr) {
        const char* ic = hasFocus ? (inEditMode ? editCursorIcon : cursorIcon) : " ";
        gDisplay->setCursor(x, y);
        gDisplay->draw(ic);
        x += gDisplay->getTextWidth(ic);
    }

    const char* textPtr = text;
    if (hasFocus) {
        uint8_t len = strlen(text);
        textPtr = viewShift < len ? text + viewShift : "";
    }
    gDisplay->setCursor(x, y);
    gDisplay->draw(textPtr);
    x += gDisplay->getTextWidth(textPtr);

    if (value) {
        gDisplay->setCursor(x, y);
        gDisplay->draw(":");
        x += gDisplay->getTextWidth(":");
        const char* valPtr = value;
        if (hasFocus) {
            uint8_t textLen = strlen(text);
            if (viewShift > textLen) {
                uint8_t shift = viewShift - textLen - 1;
                valPtr = shift < strlen(value) ? value + shift : "";
            }
        }
        gDisplay->setCursor(x, y);
        gDisplay->draw(valPtr);
        x += gDisplay->getTextWidth(valPtr);
    }

    if (hasFocus) moveCursor((displayWidth - scrollbarWidth) / charWidth, cursorRow);

    if (cursorRow == 0) drawScrollBar();
}

void GraphicalDisplayRenderer::clearBlinker() {}

void GraphicalDisplayRenderer::drawBlinker() { gDisplay->sendBuffer(); }

void GraphicalDisplayRenderer::moveCursor(uint8_t col, uint8_t row) {
    MenuRenderer::moveCursor(col, row);
    gDisplay->setCursor(col * charWidth, (row + 1) * charHeight);
}

uint8_t GraphicalDisplayRenderer::getEffectiveCols() const {
    return (displayWidth - scrollbarWidth) / charWidth -
           ((cursorIcon != nullptr && cursorIcon[0] != '\0') ||
                    (editCursorIcon != nullptr && editCursorIcon[0] != '\0')
                ? 1
                : 0);
}

void GraphicalDisplayRenderer::drawScrollBar() {
    if (totalItems <= maxRows) return;
    uint8_t x = displayWidth - scrollbarWidth;
    gDisplay->drawFrame(x, 0, scrollbarWidth, displayHeight);
    float ratio = (float)maxRows / totalItems;
    uint8_t h = ratio * displayHeight;
    if (h < 2) h = 2;
    float posRatio = 0.0f;
    if (totalItems > maxRows) posRatio = (float)viewStart / (totalItems - maxRows);
    uint8_t y = posRatio * (displayHeight - h);
    gDisplay->drawBox(x, y, scrollbarWidth, h);
}
