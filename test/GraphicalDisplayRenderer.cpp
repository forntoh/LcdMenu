#include <ArduinoUnitTests.h>
#include <display/GraphicalDisplayInterface.h>
#include <renderer/GraphicalDisplayRenderer.h>
#include <renderer/GraphicalValueSelectionRenderer.h>

class StubGraphicalDisplay : public GraphicalDisplayInterface {
  public:
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
    void setDrawColor(uint8_t) override {}
    void clearBuffer() override {}
    void sendBuffer() override {}
    void drawBox(uint8_t, uint8_t, uint8_t, uint8_t) override {}
    void drawFrame(uint8_t, uint8_t, uint8_t, uint8_t) override {}
    void drawXbm(uint8_t, uint8_t, uint8_t, uint8_t, const uint8_t*) override {}
};

unittest(graphical_renderer_exposes_value_selection_extension) {
    StubGraphicalDisplay display;
    GraphicalDisplayRenderer renderer(&display);

    void* extension = renderer.queryExtension(GraphicalValueSelectionRenderer::extensionId());
    assertTrue(extension != NULL);

    GraphicalValueSelectionRenderer* selection = static_cast<GraphicalValueSelectionRenderer*>(extension);
    selection->setValueSelection(1, 2);
    selection->clearValueSelection();
}

unittest_main()
