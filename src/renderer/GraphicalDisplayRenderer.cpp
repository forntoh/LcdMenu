#include "GraphicalDisplayRenderer.h"

GraphicalDisplayRenderer::GraphicalDisplayRenderer(
    GraphicalDisplayInterface* display,
    uint8_t chWidth)
    : MenuRenderer(display,
                   (display->getDisplayWidth() - scrollbarWidth) / chWidth,
                   display->getDisplayHeight() / (display->getFontHeight() + 2)),
      gDisplay(display),
      charWidth(chWidth),
      displayWidth(display->getDisplayWidth()),
      displayHeight(display->getDisplayHeight()) {}

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
    uint8_t fontHeight = gDisplay->getFontHeight();
    uint8_t rowHeight = fontHeight + 2;
    uint8_t top = cursorRow * rowHeight;
    uint8_t y = top + fontHeight;
    uint8_t textAreaWidth = displayWidth - scrollbarWidth - 1;

    if (hasFocus) {
        gDisplay->setDrawColor(1);
        gDisplay->drawBox(0, top, textAreaWidth, rowHeight);
        gDisplay->setDrawColor(0);
    } else {
        gDisplay->setDrawColor(1);
    }

    gDisplay->setCursor(0, y);
    const char* textPtr = text;
    if (hasFocus) {
        uint8_t len = strlen(text);
        textPtr = viewShift < len ? text + viewShift : "";
    }
    gDisplay->draw(textPtr);

    if (value) {
        uint8_t valX = labelWidth + gutter;
        gDisplay->setCursor(valX, y);
        gDisplay->draw(":");
        valX += gDisplay->getTextWidth(":");
        const char* valPtr = value;
        if (hasFocus) {
            uint8_t textLen = strlen(text);
            if (viewShift > textLen) {
                uint8_t shift = viewShift - textLen - 1;
                valPtr = shift < strlen(value) ? value + shift : "";
            }
        }
        gDisplay->setCursor(valX, y);
        gDisplay->draw(valPtr);
    }

    gDisplay->setDrawColor(1);

    if (hasFocus) moveCursor((displayWidth - scrollbarWidth) / charWidth, cursorRow);

    if (cursorRow == 0) drawScrollBar();
}

void GraphicalDisplayRenderer::clearBlinker() {}

void GraphicalDisplayRenderer::drawBlinker() { gDisplay->sendBuffer(); }

void GraphicalDisplayRenderer::moveCursor(uint8_t col, uint8_t row) {
    MenuRenderer::moveCursor(col, row);
    uint8_t fontHeight = gDisplay->getFontHeight();
    uint8_t rowHeight = fontHeight + 2;
    gDisplay->setCursor(col * charWidth, row * rowHeight + fontHeight);
}

void GraphicalDisplayRenderer::drawSubMenuIndicator() {
    uint8_t fontHeight = gDisplay->getFontHeight();
    uint8_t rowHeight = fontHeight + 2;
    uint8_t y = cursorRow * rowHeight + fontHeight;
    uint8_t arrowWidth = gDisplay->getTextWidth("\u25B8");
    uint8_t x = displayWidth - scrollbarWidth - arrowWidth - 1;
    gDisplay->setCursor(x, y);
    if (hasFocus) {
        gDisplay->setDrawColor(0);
        gDisplay->draw("\u25B8");
        gDisplay->setDrawColor(1);
    } else {
        gDisplay->draw("\u25B8");
    }
}

uint8_t GraphicalDisplayRenderer::getEffectiveCols() const {
    return (displayWidth - scrollbarWidth) / charWidth;
}

void GraphicalDisplayRenderer::drawScrollBar() {
    if (totalItems <= maxRows) return;
    uint8_t x = displayWidth - scrollbarWidth;
    float ratio = (float)maxRows / totalItems;
    uint8_t h = ratio * displayHeight;
    if (h < 2) h = 2;
    float posRatio = 0.0f;
    if (totalItems > maxRows) posRatio = (float)viewStart / (totalItems - maxRows);
    uint8_t y = posRatio * (displayHeight - h);
    gDisplay->drawBox(x, y, scrollbarWidth, h);
}
