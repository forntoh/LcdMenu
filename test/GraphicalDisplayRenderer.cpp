#include <ArduinoUnitTests.h>
#include <display/GraphicalDisplayInterface.h>
#include <renderer/GraphicalDisplayRenderer.h>
#include <renderer/GraphicalValueSelectionRenderer.h>

#include <string.h>

class StubGraphicalDisplay : public GraphicalDisplayInterface {
  public:
    uint8_t displayWidth = 128;
    uint8_t displayHeight = 64;
    uint8_t fontWidth = 6;
    uint8_t fontHeight = 8;
    uint8_t drawBoxCount = 0;
    uint8_t drawXbmCount = 0;
    uint8_t drawFrameCount = 0;
    uint8_t drawColorCount = 0;
    uint8_t drawColors[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    uint8_t lastDrawBoxHeight = 0;
    uint8_t lastDrawBoxY = 0;
    uint8_t lastDrawFrameWidth = 0;
    uint8_t lastCursorX = 0;
    uint8_t lastCursorY = 0;
    uint8_t drawTextCount = 0;
    char drawTextLog[4][16] = {{0}};

    void begin() override {}
    void clear() override {}
    void show() override {}
    void hide() override {}
    void draw(uint8_t) override {}
    void draw(const char* text) override {
        if (drawTextCount < 4) {
            uint8_t i = 0;
            while (text != NULL && text[i] != '\0' && i < 15) {
                drawTextLog[drawTextCount][i] = text[i];
                i++;
            }
            drawTextLog[drawTextCount][i] = '\0';
        }
        drawTextCount++;
    }
    void setCursor(uint8_t x, uint8_t y) override {
        lastCursorX = x;
        lastCursorY = y;
    }
    void setBacklight(bool) override {}

    void setFont(const uint8_t*) override {}
    uint8_t getDisplayWidth() const override { return displayWidth; }
    uint8_t getDisplayHeight() const override { return displayHeight; }
    uint8_t getFontWidth() const override { return fontWidth; }
    uint8_t getFontHeight() const override { return fontHeight; }
    uint8_t getTextWidth(const char* text) override {
        if (text == NULL) {
            return 0;
        }
        uint8_t len = 0;
        while (text[len] != '\0') {
            len++;
        }
        return static_cast<uint8_t>(len * fontWidth);
    }
    void setDrawColor(uint8_t color) override {
        if (drawColorCount < 8) {
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
    void drawFrame(uint8_t, uint8_t, uint8_t w, uint8_t) override {
        drawFrameCount++;
        lastDrawFrameWidth = w;
    }
    void drawXbm(uint8_t, uint8_t, uint8_t, uint8_t, const uint8_t*) override {
        drawXbmCount++;
    }
};

class FocusableGraphicalDisplayRenderer : public GraphicalDisplayRenderer {
  public:
    using GraphicalDisplayRenderer::GraphicalDisplayRenderer;

    void setFocusForTest(bool focused) {
        hasFocus = focused;
    }
};

class ToggleGraphicalItem : public BasicItem {
  public:
    explicit ToggleGraphicalItem(const char* text) : BasicItem(text) {}
    bool hasGraphicalToggle() const override { return true; }
    bool graphicalToggleState() const override { return true; }
};

unittest(graphical_renderer_draws_placeholder_cursor_for_unfocused_rows) {
    StubGraphicalDisplay display;
    GraphicalDisplayRenderer renderer(&display, NULL, ">>", "[]");

    renderer.drawItem("Label", NULL);

    assertTrue(display.drawTextCount >= 2);
    assertTrue(strcmp(display.drawTextLog[0], " ") == 0);
    assertTrue(strcmp(display.drawTextLog[1], "Label") == 0);
}

unittest(graphical_renderer_exposes_value_selection_extension) {
    StubGraphicalDisplay display;
    GraphicalDisplayRenderer renderer(&display);

    void* extension = renderer.queryExtension(GraphicalValueSelectionRenderer::extensionId());
    assertTrue(extension != NULL);

    GraphicalValueSelectionRenderer* selection =
        static_cast<GraphicalValueSelectionRenderer*>(extension);
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

unittest(graphical_renderer_toggle_keeps_minimum_box_width) {
    StubGraphicalDisplay display;
    display.fontHeight = 4;

    FocusableGraphicalDisplayRenderer renderer(&display);
    ToggleGraphicalItem toggleItem("Toggle");

    renderer.setFocusForTest(true);
    renderer.setActiveItem(&toggleItem);
    renderer.drawItem("Toggle", NULL);

    assertEqual(1, display.drawFrameCount);
    assertEqual(4, display.lastDrawFrameWidth);
}

unittest(graphical_renderer_does_not_reserve_third_for_short_value) {
    StubGraphicalDisplay display;
    display.displayWidth = 48;
    GraphicalDisplayRenderer renderer(&display);

    renderer.drawItem("ABCDEFGHIJ", "1");

    assertTrue(display.drawTextCount >= 2);
    assertTrue(strcmp(display.drawTextLog[0], "ABCDEF") == 0);
    assertTrue(strcmp(display.drawTextLog[1], "1") == 0);
}

unittest(graphical_renderer_clears_scrollbar_track_before_handle) {
    StubGraphicalDisplay display;
    GraphicalDisplayRenderer renderer(&display);

    renderer.setViewportContext(0, 9);
    renderer.drawItem("Label", NULL);

    assertEqual(3, display.drawBoxCount);
    assertEqual(0, display.drawColors[0]);
    assertEqual(1, display.drawColors[1]);
    assertTrue(display.drawColorCount >= 4);
    assertEqual(0, display.drawColors[display.drawColorCount - 2]);
    assertEqual(1, display.drawColors[display.drawColorCount - 1]);
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
