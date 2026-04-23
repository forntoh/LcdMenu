#pragma once

#include "FrameLifecycleRenderer.h"
#include "GraphicalIndicatorRenderer.h"
#include "GraphicalItemFont.h"
#include "GraphicalRendererContext.h"
#include "MenuRenderer.h"
#include "display/GraphicalDisplayInterface.h"
#include "utils/std.h"

/**
 * @class GraphicalDisplayRenderer
 * @brief Renderer for pixel-addressable displays (for example U8g2-backed).
 */
class GraphicalDisplayRenderer : public MenuRenderer,
                                 public FrameLifecycleRenderer,
                                 public GraphicalIndicatorRenderer,
                                 public GraphicalRendererContext {
  private:
    GraphicalDisplayInterface* gDisplay;
    const uint8_t* defaultFont = NULL;

    const MenuItem* activeItem = NULL;
    uint8_t valueAreaWidth = 0;
    uint8_t viewStart = 0;
    uint8_t totalItems = 0;

    uint8_t cursorPixelX = 0;
    uint8_t cursorPixelY = 0;
    uint8_t maxRowHeight = 8;
    uint8_t maxFontWidth = 1;

    const char* cursorIcon;
    const char* editCursorIcon;

    static const uint8_t scrollbarWidth = 1;
    static const uint8_t scrollbarGap = 1;
    static const uint8_t leftPadding = 1;
    static const uint8_t rightPadding = 1;
    static const uint8_t cursorGap = 1;
    static const uint8_t listGlyphWidth = 7;
    static const uint8_t listGlyphHeight = 8;
    static const uint8_t listGap = 1;
    static const uint8_t submenuGlyphWidth = 3;
    static const uint8_t submenuGlyphHeight = 5;

    void captureCurrentFontMetrics();
    void applyItemFont(const MenuItem* item);

    uint8_t measureText(const char* text) const;
    uint8_t toggleIndicatorWidth() const;
    uint8_t rowHeight() const;
    void drawScrollBar();

    uint8_t getMaxRows() const override;
    uint8_t getMaxCols() const override;
    uint8_t getEffectiveCols() const override;

  public:
    explicit GraphicalDisplayRenderer(
        GraphicalDisplayInterface* display,
        const uint8_t* defaultFont = NULL,
        const char* cursorIcon = NULL,
        const char* editCursorIcon = NULL);

    void setDefaultFont(const uint8_t* font);
    bool setItemFont(MenuItem* item, const uint8_t* font);

    void begin() override;
    void beginFrame() override;
    void endFrame() override;

    void setViewportContext(uint8_t viewStart, uint8_t totalItems) override;
    void setValueAreaWidth(uint8_t width) override;
    void setActiveItem(const MenuItem* item) override;
    GraphicalDisplayInterface* getGraphicalDisplay() override;

    void* queryExtension(uint8_t extensionId) override;
    const void* queryExtension(uint8_t extensionId) const override;

    void draw(uint8_t byte) override;
    void drawItem(const char* text, const char* value, bool padWithBlanks = true) override;
    void clearBlinker() override;
    void drawBlinker() override;
    void moveCursor(uint8_t cursorCol, uint8_t cursorRow) override;
    void drawSubMenuIndicator() override;
    void drawListIndicator() override;
};
