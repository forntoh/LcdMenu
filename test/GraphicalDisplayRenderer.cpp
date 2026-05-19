#include <ArduinoUnitTests.h>
#include <display/GraphicalDisplayInterface.h>
#include <renderer/GraphicalDisplayRenderer.h>
#include <renderer/GraphicalValueSelectionRenderer.h>

class StubGraphicalDisplay : public GraphicalDisplayInterface {
  public:
    uint8_t drawBoxCount = 0;
    uint8_t drawXbmCount = 0;
    uint8_t drawColorCount = 0;
    uint8_t drawColors[4] = {0, 0, 0, 0};
    uint8_t lastDrawBoxHeight = 0;
    uint8_t lastDrawBoxY = 0;

    void begin() override {}
    void clear() override {}
    void show() override {}
    void hide() override {}
    void draw(uint8_t) override {}
    void draw(const char*) override {}
    void setCursor(uint8_t, uint8_t) override {}
    void setBacklight(bool) override {}

    void setFont(const uint8_t*) override {}
    uint8_t getDisplayWidth() const override { return 128; }
    uint8_t getDisplayHeight() const override { return 64; }
    uint8_t getFontWidth() const override { return 6; }
    uint8_t getFontHeight() const override { return 8; }
    uint8_t getTextWidth(const char* text) override {
        if (text == NULL) {
            return 0;
        }
        uint8_t len = 0;
        while (text[len] != '\0') {
            len++;
        }
        return static_cast<uint8_t>(len * 6);
    }
    void setDrawColor(uint8_t color) override {
        if (drawColorCount < 4) {
            drawColors[drawColorCount] = color;
        }
        drawColorCount++;
    }
    void clearBuffer() override {}
    void sendBuffer() override {}
    void drawBox(uint8_t, uint8_t y, uint8_t, uint8_t h) override {
        drawBoxCount++;
        lastDrawBoxY = y;
        lastDrawBoxHeight = h;
    }
    void drawFrame(uint8_t, uint8_t, uint8_t, uint8_t) override {}
    void drawXbm(uint8_t, uint8_t, uint8_t, uint8_t, const uint8_t*) override {
        drawXbmCount++;
    }
};

class FocusableGraphicalDisplayRenderer : public GraphicalDisplayRenderer {
  public:
    using GraphicalDisplayRenderer::GraphicalDisplayRenderer;
    using GraphicalDisplayRenderer::getEffectiveCols;
    using GraphicalDisplayRenderer::getMaxCols;

    void setFocusForTest(bool focused) {
        hasFocus = focused;
    }
};

unittest(graphical_renderer_reports_effective_cols_with_cursor_icons) {
    StubGraphicalDisplay display;

    FocusableGraphicalDisplayRenderer plain(&display);
    FocusableGraphicalDisplayRenderer iconed(&display, NULL, "[]", "[e]");

    assertEqual(21, plain.getEffectiveCols());
    assertEqual(18, iconed.getEffectiveCols());

    iconed.setViewportContext(0, 9);
    assertEqual(17, iconed.getEffectiveCols());
}

unittest(graphical_renderer_reports_max_cols_from_font_width) {
    StubGraphicalDisplay display;
    FocusableGraphicalDisplayRenderer renderer(&display);

    renderer.begin();

    assertEqual(21, renderer.getMaxCols());
}

unittest(graphical_renderer_exposes_value_selection_extension) {
    StubGraphicalDisplay display;
    GraphicalDisplayRenderer renderer(&display);

    void* extension = renderer.queryExtension(GraphicalValueSelectionRenderer::extensionId());
    assertTrue(extension != NULL);

    GraphicalValueSelectionRenderer* selection = static_cast<GraphicalValueSelectionRenderer*>(extension);
    selection->setValueSelection(1, 2);
    selection->clearValueSelection();
}

unittest(graphical_renderer_uses_tight_row_height) {
    StubGraphicalDisplay display;
    GraphicalDisplayRenderer renderer(&display);

    renderer.drawItem("Label", NULL);

    assertEqual(1, display.drawBoxCount);
    assertEqual(8, display.lastDrawBoxHeight);
}

unittest(graphical_renderer_clears_scrollbar_track_before_handle) {
    StubGraphicalDisplay display;
    GraphicalDisplayRenderer renderer(&display);

    renderer.setViewportContext(0, 9);
    renderer.drawItem("Label", NULL);

    assertEqual(3, display.drawBoxCount);
    assertEqual(0, display.drawColors[0]);
    assertEqual(1, display.drawColors[1]);
    assertEqual(0, display.drawColors[2]);
    assertEqual(1, display.drawColors[3]);
    assertEqual(0, display.lastDrawBoxY);
}

unittest(graphical_renderer_colors_indicators_when_focused) {
    StubGraphicalDisplay display;
    FocusableGraphicalDisplayRenderer renderer(&display);

    renderer.setFocusForTest(true);
    renderer.drawListIndicator();

    assertEqual(2, display.drawColorCount);
    assertEqual(0, display.drawColors[0]);
    assertEqual(1, display.drawColors[1]);
    assertEqual(1, display.drawXbmCount);

    display.drawColorCount = 0;
    display.drawXbmCount = 0;
    display.drawBoxCount = 0;

    renderer.drawSubMenuIndicator();

    assertEqual(2, display.drawColorCount);
    assertEqual(0, display.drawColors[0]);
    assertEqual(1, display.drawColors[1]);
    assertEqual(5, display.drawBoxCount);
}

unittest_main()
