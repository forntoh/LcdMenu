#define protected public
#include "Godmode.h"
#include <ItemInput.h>
#include <MenuScreen.h>
#undef protected
#include <ArduinoUnitTests.h>
#include <ItemCommand.h>
#include <ItemLabel.h>
#include <ItemToggle.h>
#include <MenuItem.h>
#include <display/DisplayInterface.h>
#include <display/GraphicalDisplayInterface.h>
#include <renderer/FrameLifecycleRenderer.h>
#include <renderer/GraphicalMenuItem.h>
#include <renderer/GraphicalValueSelectionRenderer.h>
#include <renderer/MenuRenderer.h>
#include <string.h>

#define LCD_ROWS 2
#define LCD_COLS 16

#define ITEM_MAIN_HEADER_INDEX 0
#define ITEM_INPUT_INDEX 0
#define ITEM_COMMAND_INDEX 3
#define ITEM_TOGGLE_INDEX 4
#define ITEM_LIST_INDEX 99

void commandCallback() {}
void toggleCallback(bool) {}

class StubDisplay : public DisplayInterface {
  public:
    void begin() override {}
    void clear() override {}
    void show() override {}
    void hide() override {}
    void draw(uint8_t) override {}
    void draw(const char*) override {}
    void setCursor(uint8_t, uint8_t) override {}
    void setBacklight(bool) override {}
};

class StubRenderer : public MenuRenderer {
  public:
    StubDisplay display;
    StubRenderer() : MenuRenderer(&display, LCD_COLS, LCD_ROWS) {}

    void draw(uint8_t) override {}
    void drawItem(const char*, const char*, bool) override {}
    void clearBlinker() override {}
    void drawBlinker() override {}
    uint8_t getEffectiveCols() const override { return maxCols; }
};

class TrackingDisplay : public DisplayInterface {
  public:
    bool cleared = false;
    void begin() override {}
    void clear() override { cleared = true; }
    void show() override {}
    void hide() override {}
    void draw(uint8_t) override {}
    void draw(const char*) override {}
    void setCursor(uint8_t, uint8_t) override {}
    void setBacklight(bool) override {}
};

class TrackingRenderer : public MenuRenderer, public FrameLifecycleRenderer {
  public:
    TrackingDisplay display;
    bool itemDrawn = false;
    uint8_t beginFrameCalls = 0;
    uint8_t endFrameCalls = 0;
    TrackingRenderer() : MenuRenderer(&display, LCD_COLS, LCD_ROWS) {}

    void draw(uint8_t) override {}
    void drawItem(const char*, const char*, bool) override { itemDrawn = true; }
    void clearBlinker() override {}
    void drawBlinker() override {}
    uint8_t getEffectiveCols() const override { return maxCols; }
    void beginFrame() override { beginFrameCalls++; }
    void endFrame() override { endFrameCalls++; }

    void* queryExtension(uint8_t extensionId) override {
        if (extensionId == FrameLifecycleRenderer::extensionId()) {
            return static_cast<FrameLifecycleRenderer*>(this);
        }
        return MenuRenderer::queryExtension(extensionId);
    }

    const void* queryExtension(uint8_t extensionId) const override {
        if (extensionId == FrameLifecycleRenderer::extensionId()) {
            return static_cast<const FrameLifecycleRenderer*>(this);
        }
        return MenuRenderer::queryExtension(extensionId);
    }
};

class GraphicalMeasureDisplay : public GraphicalDisplayInterface {
  public:
    static const uint8_t kCharWidth = 6;
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
    uint8_t getFontWidth() const override { return kCharWidth; }
    uint8_t getFontHeight() const override { return 8; }
    uint8_t getTextWidth(const char* text) override {
        if (text == NULL) {
            return 0;
        }
        return static_cast<uint8_t>(strlen(text) * kCharWidth);
    }
    void setDrawColor(uint8_t) override {}
    void clearBuffer() override {}
    void sendBuffer() override {}
    void drawBox(uint8_t, uint8_t, uint8_t, uint8_t) override {}
    void drawFrame(uint8_t, uint8_t, uint8_t, uint8_t) override {}
    void drawXbm(uint8_t, uint8_t, uint8_t, uint8_t, const uint8_t*) override {}
};

class SelectionTrackingRenderer : public MenuRenderer, public GraphicalValueSelectionRenderer {
  public:
    StubDisplay display;
    uint8_t blinkerDrawCalls = 0;
    uint8_t selectionStart = 0;
    uint8_t selectionLength = 0;
    bool hasSelection = false;

    SelectionTrackingRenderer() : MenuRenderer(&display, LCD_COLS, LCD_ROWS) {}

    void draw(uint8_t) override {}
    void drawItem(const char*, const char*, bool) override {}
    void clearBlinker() override {}
    void drawBlinker() override { blinkerDrawCalls++; }
    uint8_t getEffectiveCols() const override { return maxCols; }

    void setValueSelection(uint8_t start, uint8_t length) override {
        selectionStart = start;
        selectionLength = length;
        hasSelection = length > 0;
    }

    void clearValueSelection() override {
        selectionStart = 0;
        selectionLength = 0;
        hasSelection = false;
    }

    void* queryExtension(uint8_t extensionId) override {
        if (extensionId == GraphicalValueSelectionRenderer::extensionId()) {
            return static_cast<GraphicalValueSelectionRenderer*>(this);
        }
        return MenuRenderer::queryExtension(extensionId);
    }

    const void* queryExtension(uint8_t extensionId) const override {
        if (extensionId == GraphicalValueSelectionRenderer::extensionId()) {
            return static_cast<const GraphicalValueSelectionRenderer*>(this);
        }
        return MenuRenderer::queryExtension(extensionId);
    }
};

class PollingMenuItem : public MenuItem {
  public:
    bool wasDrawn = false;

    PollingMenuItem() : MenuItem("Polled") {
        polling = true;
    }

    void draw(MenuRenderer* renderer) override {
        wasDrawn = true;
        renderer->drawItem(text, NULL);
    }
};

// clang-format off
MENU_SCREEN(mainScreen, mainItems,
    ITEM_INPUT("Random", NULL),
    ITEM_INPUT("Connect", NULL),
    ITEM_BASIC("Settings"),
    ITEM_COMMAND("Backlight", commandCallback),
    ITEM_TOGGLE("Toggle", "ON", "OFF", toggleCallback),
    ITEM_BASIC("Blink random"));
// clang-format on

unittest(text_set_correctly) {
    assertEqual("Random", mainItems[ITEM_INPUT_INDEX]->getText());
}

unittest(text_off_on_for_item_toggle) {
    assertEqual("ON", (static_cast<ItemToggle*>(mainItems[ITEM_TOGGLE_INDEX]))->getTextOn());
    assertEqual("OFF", (static_cast<ItemToggle*>(mainItems[ITEM_TOGGLE_INDEX]))->getTextOff());
}

unittest(can_set_input_value) {
    char* expected = "TEST";
    assertEqual("", (static_cast<ItemInput*>(mainItems[ITEM_INPUT_INDEX]))->getValue());
    (static_cast<ItemInput*>(mainItems[ITEM_INPUT_INDEX]))->setValue(expected);
    assertEqual(expected, (static_cast<ItemInput*>(mainItems[ITEM_INPUT_INDEX]))->getValue());
}

unittest(input_item_exposes_graphical_capability) {
    GraphicalMeasureDisplay display;
    char value[] = "AB";
    ItemInput item("Name", value, NULL);

    const GraphicalMenuItem* capability = static_cast<const GraphicalMenuItem*>(item.queryCapability(GraphicalMenuItem::capabilityId()));
    assertTrue(capability != NULL);
    assertTrue(capability->useTightGraphicalSelectionBox());
    assertEqual((uint8_t)(2 * GraphicalMeasureDisplay::kCharWidth), capability->measureGraphicalValueWidth(&display));
}

unittest(input_item_uses_graphical_selection_extension_in_edit_mode) {
    char value[] = "TEST";
    ItemInput item("Name", value, NULL);
    SelectionTrackingRenderer renderer;
    LcdMenu menu(renderer);

    assertTrue(item.process(&menu, ENTER));
    assertTrue(MenuItem::isEditing());
    assertEqual((uint8_t)3, item.cursor);
    assertEqual((uint8_t)0, renderer.blinkerDrawCalls);

    assertTrue(item.process(&menu, LEFT));
    assertEqual((uint8_t)2, item.cursor);

    assertTrue(item.process(&menu, BACK));
    assertFalse(MenuItem::isEditing());
}

unittest(cursor_clamped_when_out_of_range) {
    StubRenderer renderer;
    uint8_t outOfRange = 100;
    mainScreen->setCursor(&renderer, outOfRange);
    assertEqual(mainScreen->size() - 1, mainScreen->getCursor());
}

unittest(clear_command_empties_input_and_resets_cursor) {
    char value[] = "HELLO";
    ItemInput item("Name", value, NULL);
    StubRenderer renderer;
    LcdMenu menu(renderer);

    assertTrue(item.process(&menu, ENTER));
    assertTrue(item.process(&menu, CLEAR));

    assertEqual("", item.getValue());
    assertEqual((uint8_t)0, item.cursor);
    assertEqual((uint8_t)0, item.view);
    assertTrue(MenuItem::isEditing());
}

unittest(hide_disables_and_clears_display) {
    TrackingRenderer renderer;
    LcdMenu menu(renderer);
    menu.setScreen(mainScreen);
    renderer.display.cleared = false;
    menu.hide();
    assertFalse(menu.isEnabled());
    assertTrue(renderer.display.cleared);
}

unittest(show_enables_and_draws_active_screen) {
    TrackingRenderer renderer;
    LcdMenu menu(renderer);
    menu.setScreen(mainScreen);
    menu.hide();
    renderer.display.cleared = false;
    renderer.itemDrawn = false;
    menu.show();
    assertTrue(menu.isEnabled());
    assertTrue(renderer.display.cleared);
    assertTrue(renderer.itemDrawn);
}

unittest(refresh_flushes_renderer_frame) {
    TrackingRenderer renderer;
    LcdMenu menu(renderer);
    menu.setScreen(mainScreen);

    renderer.beginFrameCalls = 0;
    renderer.endFrameCalls = 0;
    menu.refresh();

    assertEqual((uint8_t)1, renderer.beginFrameCalls);
    assertEqual((uint8_t)1, renderer.endFrameCalls);
}

unittest(process_flushes_renderer_when_back_navigates_and_redraws) {
    TrackingRenderer renderer;
    LcdMenu menu(renderer);
    MenuItem* parentItem = ITEM_BASIC("Parent");
    MenuItem* childItem = ITEM_BASIC("Child");
    std::vector<MenuItem*> parentItems = {parentItem};
    std::vector<MenuItem*> childItems = {childItem};
    MenuScreen parent(parentItems);
    MenuScreen child(childItems);
    child.setParent(&parent);
    menu.setScreen(&child);

    MenuItem::endEdit();

    renderer.beginFrameCalls = 0;
    renderer.endFrameCalls = 0;

    assertTrue(menu.process(BACK));
    assertEqual((uint8_t)1, renderer.beginFrameCalls);
    assertEqual((uint8_t)1, renderer.endFrameCalls);

    delete parentItem;
    delete childItem;
}

unittest(poll_flushes_renderer_when_polled_item_redraws) {
    TrackingRenderer renderer;
    LcdMenu menu(renderer);
    PollingMenuItem polledItem;
    std::vector<MenuItem*> items = {&polledItem};
    MenuScreen screen(items);

    menu.setScreen(&screen);

    MenuItem::endEdit();
    GODMODE()->micros += 200000;

    polledItem.wasDrawn = false;
    renderer.endFrameCalls = 0;
    menu.poll(100);

    assertTrue(polledItem.wasDrawn);
    assertEqual((uint8_t)1, renderer.endFrameCalls);
}

unittest(set_screen_skips_initial_label) {
    MenuItem* label = ITEM_LABEL("Title");
    MenuItem* item = ITEM_BASIC("Run");
    std::vector<MenuItem*> items = {label, item};
    MenuScreen screen(items);
    TrackingRenderer renderer;
    LcdMenu menu(renderer);
    menu.setScreen(&screen);
    assertEqual((uint8_t)1, menu.getCursor());
    delete label;
    delete item;
}

unittest_main()
