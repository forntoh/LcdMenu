#pragma once

#include "MenuRenderer.h"
#include "display/GraphicalDisplayInterface.h"

class GraphicalDisplayRenderer : public MenuRenderer {
  private:
    GraphicalDisplayInterface* gDisplay;
    const uint8_t* font;
    static constexpr uint8_t scrollbarWidth = 1;
    static constexpr uint8_t gutter = 2;
    static constexpr uint8_t valueOffsetRight = 1;
    static constexpr uint8_t listGlyphWidth = 7;
    static constexpr uint8_t listGlyphHeight = 8;
    static constexpr uint8_t listGap = 1;
    uint8_t valueWidth = 0;
    uint8_t highlightOffset = 0;
    bool drawListNext = false;

    void drawScrollBar();
    uint8_t getEffectiveCols() const override;
    uint8_t getMaxRows() const override {
        uint8_t fh = gDisplay->getFontHeight();
        if (fh == 0) return 0;
        return gDisplay->getDisplayHeight() / fh;
    }
    uint8_t getMaxCols() const override {
        uint8_t fw = gDisplay->getFontWidth();
        if (fw == 0) return 0;
        return (gDisplay->getDisplayWidth() - scrollbarWidth) / fw;
    }

  public:
    explicit GraphicalDisplayRenderer(GraphicalDisplayInterface* display,
                                      const uint8_t* font = nullptr);

    void begin() override;
    uint8_t draw(uint8_t byte) override;
    void drawItem(const char* text, const char* value, bool lastValue = true) override;
    void clearBlinker() override;
    void drawBlinker() override;
    void moveCursor(uint8_t cursorCol, uint8_t cursorRow) override;
    void drawSubMenuIndicator() override;
    void drawListIndicator() override;

    void setHighlightValue(bool enable) override { MenuRenderer::highlightValue = enable; }

    void setNextListIndicator(bool enable) override { drawListNext = enable; }

    void setHighlightOffset(uint8_t offset) { highlightOffset = offset; }

    void setValueWidth(uint8_t width) { valueWidth = width; }
    uint8_t getCharWidth() const { return gDisplay->getFontWidth(); }
    uint8_t getDisplayWidth() const { return gDisplay->getDisplayWidth(); }
    uint8_t getDisplayHeight() const { return gDisplay->getDisplayHeight(); }
};
