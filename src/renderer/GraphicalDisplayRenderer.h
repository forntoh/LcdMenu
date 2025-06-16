#pragma once

#include "MenuRenderer.h"
#include "display/GraphicalDisplayInterface.h"

class GraphicalDisplayRenderer : public MenuRenderer {
  private:
    GraphicalDisplayInterface* gDisplay;
    static constexpr uint8_t scrollbarWidth = 1;
    static constexpr uint8_t gutter = 2;
    uint8_t labelWidth = 0;

    void drawScrollBar();
    uint8_t getEffectiveCols() const override;
    uint8_t getMaxRows() const override {
        return gDisplay->getDisplayHeight() / gDisplay->getFontHeight();
    }
    uint8_t getMaxCols() const override { return (gDisplay->getDisplayWidth() - scrollbarWidth) / gDisplay->getFontWidth(); }

  public:
    explicit GraphicalDisplayRenderer(GraphicalDisplayInterface* display);

    void begin() override;
    void draw(uint8_t byte) override;
    void drawItem(const char* text, const char* value, bool padWithBlanks = true) override;
    void clearBlinker() override;
    void drawBlinker() override;
    void moveCursor(uint8_t cursorCol, uint8_t cursorRow) override;
    void drawSubMenuIndicator() override;

    void setLabelWidth(uint8_t width) { labelWidth = width; }
    uint8_t getCharWidth() const { return gDisplay->getFontWidth(); }
    uint8_t getDisplayWidth() const { return gDisplay->getDisplayWidth(); }
    uint8_t getDisplayHeight() const { return gDisplay->getDisplayHeight(); }
};
