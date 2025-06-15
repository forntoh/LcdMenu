#pragma once

#include "MenuRenderer.h"
#include "display/GraphicalDisplayInterface.h"

class GraphicalDisplayRenderer : public MenuRenderer {
  private:
    GraphicalDisplayInterface* gDisplay;
    const uint8_t charWidth;
    const uint8_t charHeight;
    const uint8_t displayWidth;
    const uint8_t displayHeight;
    const char* cursorIcon;
    const char* editCursorIcon;
    const uint8_t scrollbarWidth;

    void drawScrollBar();
    uint8_t getEffectiveCols() const override;

  public:
    GraphicalDisplayRenderer(GraphicalDisplayInterface* display,
                             uint8_t displayWidth,
                             uint8_t displayHeight,
                             uint8_t charWidth,
                             uint8_t charHeight,
                             const char* cursorIcon = ">",
                             const char* editCursorIcon = "<",
                             uint8_t scrollbarWidth = 4);

    void begin() override;
    void draw(uint8_t byte) override;
    void drawItem(const char* text, const char* value, bool padWithBlanks = true) override;
    void clearBlinker() override;
    void drawBlinker() override;
    void moveCursor(uint8_t cursorCol, uint8_t cursorRow) override;
};
