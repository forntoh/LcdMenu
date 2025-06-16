#include "GraphicalDisplayRenderer.h"
#include <U8g2lib.h>
#include <string.h>

static const uint8_t updown_glyph[] = {
    0x08,
    0x1C,
    0x3E,
    0x00,
    0x3E,
    0x1C,
    0x08,
};

GraphicalDisplayRenderer::GraphicalDisplayRenderer(GraphicalDisplayInterface* display,
                                                   const uint8_t* font)
    : MenuRenderer(display, 0, 0), gDisplay(display), font(font) {}

void GraphicalDisplayRenderer::begin() {
    MenuRenderer::begin();
    if (font) {
        gDisplay->setFont(font);
    }
    if (gDisplay->getFontHeight() == 0) {
        extern const uint8_t u8g2_font_6x12_m_symbols[];
        gDisplay->setFont(u8g2_font_6x12_m_symbols);
    }
    drawScrollBar();
    gDisplay->sendBuffer();
}

uint8_t GraphicalDisplayRenderer::draw(uint8_t byte) {
    char c[2] = {static_cast<char>(byte), '\0'};
    return gDisplay->draw(c);
}

void GraphicalDisplayRenderer::drawItem(const char* text, const char* value, bool lastValue) {
    uint8_t fontHeight = gDisplay->getFontHeight();
    uint8_t rowHeight = fontHeight;
    uint8_t top = cursorRow * rowHeight;
    uint8_t y = top + fontHeight - 1;
    uint8_t displayWidth = gDisplay->getDisplayWidth();
    bool showScrollBar = totalItems > getMaxRows();
    uint8_t rightGap = showScrollBar ? scrollbarWidth + 1 : 0;
    uint8_t textAreaWidth = displayWidth - rightGap;

    bool highlightRow = hasFocus && !isInEditMode();
    bool highlightVal = hasFocus && isInEditMode() && highlightValue;
    if (highlightRow) {
        gDisplay->setDrawColor(1);
        gDisplay->drawBox(0, top + 1, textAreaWidth, rowHeight);
        gDisplay->setDrawColor(0);
    } else {
        gDisplay->setDrawColor(1);
    }

    gDisplay->setCursor(1, y);
    const char* textPtr = text;
    if (hasFocus) {
        uint8_t len = strlen(text);
        textPtr = viewShift < len ? text + viewShift : "";
    }
    gDisplay->draw(textPtr);

    if (value) {
        uint8_t indicatorSpace = drawListNext ? listGlyphWidth + listGap : 0;
        uint8_t valX =
            displayWidth - rightGap - valueOffsetRight - indicatorSpace -
            valueWidth;
        const char* valPtr = value;
        if (hasFocus) {
            uint8_t textLen = strlen(text);
            if (viewShift > textLen) {
                uint8_t shift = viewShift - textLen;
                valPtr = shift < strlen(value) ? value + shift : "";
            }
        }
        if (highlightVal) {
            uint8_t valW = gDisplay->getTextWidth(valPtr);
            uint8_t highlightW = valW + 2;
            if (lastValue)
                highlightW = displayWidth - rightGap - valueOffsetRight - valX;
            gDisplay->drawBox(valX - 1, top + 1, highlightW, rowHeight);
            gDisplay->setDrawColor(0);
        }
        gDisplay->setCursor(valX, y);
        gDisplay->draw(valPtr);
        if (highlightVal) gDisplay->setDrawColor(1);
    }

    gDisplay->setDrawColor(1);

    if (hasFocus)
        moveCursor((displayWidth - rightGap - valueOffsetRight -
                    (drawListNext ? listGlyphWidth + listGap : 0)) /
                       gDisplay->getFontWidth(),
                   cursorRow);

    if (cursorRow == 0) drawScrollBar();
}

void GraphicalDisplayRenderer::clearBlinker() {}

void GraphicalDisplayRenderer::drawBlinker() { gDisplay->sendBuffer(); }

void GraphicalDisplayRenderer::moveCursor(uint8_t col, uint8_t row) {
    MenuRenderer::moveCursor(col, row);
    uint8_t fontHeight = gDisplay->getFontHeight();
    uint8_t rowHeight = fontHeight;
    gDisplay->setCursor(col * gDisplay->getFontWidth(),
                        row * rowHeight + fontHeight - 1);
}

void GraphicalDisplayRenderer::drawSubMenuIndicator() {
    uint8_t fontHeight = gDisplay->getFontHeight();
    uint8_t rowHeight = fontHeight;
    uint8_t y = cursorRow * rowHeight + fontHeight - 1;
    uint8_t arrowWidth = gDisplay->getTextWidth("\u25B8");
    uint8_t displayWidth = gDisplay->getDisplayWidth();
    bool showScrollBar = totalItems > getMaxRows();
    uint8_t rightGap = showScrollBar ? scrollbarWidth + 1 : 0;
    uint8_t x = displayWidth - rightGap - arrowWidth - valueOffsetRight;
    gDisplay->setCursor(x, y);
    if (hasFocus) {
        gDisplay->setDrawColor(0);
        gDisplay->draw("\u25B8");
        gDisplay->setDrawColor(1);
    } else {
        gDisplay->draw("\u25B8");
    }
}

void GraphicalDisplayRenderer::drawListIndicator() {
    uint8_t displayWidth = gDisplay->getDisplayWidth();
    bool showScrollBar = totalItems > getMaxRows();
    uint8_t rightGap = showScrollBar ? scrollbarWidth + 1 : 0;
    uint8_t x = displayWidth - rightGap - listGlyphWidth - valueOffsetRight;
    int8_t top = cursorRow * gDisplay->getFontHeight() + (listGlyphHeight / 2);
    if (top < 0) top = 0;
    if (hasFocus) gDisplay->setDrawColor(0);
    gDisplay->drawXbm(x, top, listGlyphWidth, listGlyphHeight, updown_glyph);
    if (hasFocus) gDisplay->setDrawColor(1);
    drawListNext = false;
}

uint8_t GraphicalDisplayRenderer::getEffectiveCols() const {
    bool showScrollBar = totalItems > getMaxRows();
    uint8_t rightGap = showScrollBar ? scrollbarWidth + 1 : 0;
    return (gDisplay->getDisplayWidth() - rightGap - valueOffsetRight) /
           gDisplay->getFontWidth();
}

void GraphicalDisplayRenderer::drawScrollBar() {
    uint8_t rows = getMaxRows();
    if (totalItems <= rows) return;
    uint8_t displayWidth = gDisplay->getDisplayWidth();
    uint8_t areaHeight = getMaxRows() * gDisplay->getFontHeight() +
                         gDisplay->getFontHeight();
    if (areaHeight > gDisplay->getDisplayHeight())
        areaHeight = gDisplay->getDisplayHeight();
    uint8_t x = displayWidth - scrollbarWidth;
    float ratio = static_cast<float>(rows) / totalItems;
    uint8_t h = ratio * areaHeight;
    if (h < 2) h = 2;
    float posRatio = 0.0f;
    if (totalItems > rows) posRatio = static_cast<float>(viewStart) / (totalItems - rows);
    uint8_t y = posRatio * (areaHeight - h);
    gDisplay->drawBox(x, y, scrollbarWidth, h);
}
