// Creator: @ShishkinDmitriy
#ifndef BASE_ITEM_MANY_WIDGETS_H
#define BASE_ITEM_MANY_WIDGETS_H

#include "LcdMenu.h"
#include "MenuItem.h"
#include "display/GraphicalDisplayInterface.h"
#include "renderer/GraphicalIndicatorRenderer.h"
#include "renderer/GraphicalMenuItem.h"
#include "renderer/GraphicalValueSelectionRenderer.h"
#include "utils/lcd_menu_utils.h"
#include "utils/std.h"
#include "widget/BaseWidget.h"
#include <vector>

class BaseItemManyWidgets : public MenuItem, public GraphicalMenuItem {
  protected:
    std::vector<BaseWidget*> widgets;
    uint8_t activeWidget = 0;

  public:
    BaseItemManyWidgets(
        const char* text,
        std::vector<BaseWidget*> widgets,
        uint8_t activeWidget = 0)
        : MenuItem(text), widgets(widgets), activeWidget(constrain(activeWidget, 0, widgets.size())) {
        this->polling = true;
    }

    uint8_t getActiveWidget() const { return activeWidget; }
    void setActiveWidget(const uint8_t activeWidget) {
        if (activeWidget < widgets.size()) {
            this->activeWidget = activeWidget;
        }
    }

    BaseWidget* getWidgetAt(const uint8_t index) const {
        return index < widgets.size() ? widgets[index] : nullptr;
    }

    /**
     * @brief Add a widget to the item.
     *
     * @param widget The widget to be added.
     */
    void addWidget(BaseWidget* widget) { widgets.push_back(widget); }

    /**
     * @brief Add a widget to the item at the specified index.
     *
     * @param index The index at which to add the widget.
     * @param widget The widget to be added.
     */
    void addWidgetAt(uint8_t index, BaseWidget* widget) {
        if (index < widgets.size()) {
            widgets.insert(widgets.begin() + index, widget);
        }
    }

    /**
     * @brief Remove a widget from the item.
     *
     * This function removes a widget from the item at the specified index.
     * If the item has only one widget, it will not be removed.
     *
     * @param index The index of the widget to be removed.
     */
    void removeWidget(uint8_t index) {
        if (widgets.size() > 1 && index < widgets.size()) {
            delete widgets[index];
            widgets.erase(widgets.begin() + index);
            if (activeWidget >= widgets.size()) {
                activeWidget = widgets.size() - 1;
            }
        }
    }

    uint8_t measureGraphicalValueWidth(GraphicalDisplayInterface* display) const override {
        if (display == NULL) {
            return 0;
        }

        char buf[ITEM_DRAW_BUFFER_SIZE];
        uint8_t index = 0;
        for (size_t i = 0; i < widgets.size() && index < ITEM_DRAW_BUFFER_SIZE - 1; i++) {
            uint8_t written = widgets[i]->draw(buf, index);
            uint8_t maxWritable = static_cast<uint8_t>(ITEM_DRAW_BUFFER_SIZE - 1 - index);
            index += written > maxWritable ? maxWritable : written;
        }
        buf[index] = '\0';
        return display->getTextWidth(buf);
    }

    bool hasGraphicalListIndicator() const override {
        for (size_t i = 0; i < widgets.size(); i++) {
            if (widgets[i] != NULL && widgets[i]->isList()) {
                return true;
            }
        }
        return false;
    }

    const void* queryCapability(uint8_t capabilityId) const override {
        if (capabilityId == GraphicalMenuItem::capabilityId()) {
            return static_cast<const GraphicalMenuItem*>(this);
        }
        return MenuItem::queryCapability(capabilityId);
    }

    virtual ~BaseItemManyWidgets() {
        for (auto widget : widgets) {
            delete widget;
        }
    }

  protected:
    virtual void handleCommit() = 0;
    /**
     * @brief Reset the active widget to the first widget.
     */
    void reset() { activeWidget = 0; }

    /**
     * @brief Draws the menu item using the provided renderer.
     *
     * This function is responsible for rendering the menu item and its associated
     * widgets. It iterates through each widget, drawing them into a buffer, and
     * then uses the renderer to display the combined result. If the renderer is
     * in edit mode, it also handles the cursor positioning for editing.
     *
     * @param renderer A pointer to the MenuRenderer object used for drawing the
     * item.
     *
     * The function performs the following steps:
     * 1. Initializes a buffer to hold the drawn content and variables for
     * indexing and cursor positioning.
     * 2. Iterates through each widget, drawing it into the buffer and updating
     * the index.
     * 3. If the active widget is being edited, it draws the item using the
     * renderer and calculates the cursor position.
     * 4. Draws the final item using the renderer.
     * 5. If in edit mode, moves the cursor to the appropriate position for
     * editing.
     */
    void draw(MenuRenderer* renderer) override {
        char buf[ITEM_DRAW_BUFFER_SIZE];

        uint8_t index = 0;
        uint8_t cursorCol = 0;
        bool hasListWidget = false;
        uint8_t activeSegmentStart = 0;
        uint8_t activeSegmentLength = 0;

        GraphicalValueSelectionRenderer* valueSelectionRenderer =
            static_cast<GraphicalValueSelectionRenderer*>(renderer->queryExtension(GraphicalValueSelectionRenderer::extensionId()));

        for (uint8_t i = 0; i < widgets.size(); i++) {
            uint8_t widgetStart = index;
            uint8_t written = widgets[i]->draw(buf, index);
            uint8_t maxWritable = index < ITEM_DRAW_BUFFER_SIZE - 1 ? static_cast<uint8_t>(ITEM_DRAW_BUFFER_SIZE - 1 - index) : 0;
            index += written > maxWritable ? maxWritable : written;
            hasListWidget = hasListWidget || widgets[i]->isList();
            if (i == activeWidget && MenuItem::isEditing()) {
                activeSegmentStart = widgetStart;
                activeSegmentLength = index > widgetStart ? static_cast<uint8_t>(index - widgetStart) : 0;

                const char* label = text == NULL ? "" : text;
                int16_t valueArea = static_cast<int16_t>(renderer->getEffectiveCols()) - static_cast<int16_t>(strlen(label)) - 1;
                int16_t shift = static_cast<int16_t>(index) - valueArea;
                renderer->viewShift = shift > 0 ? static_cast<uint8_t>(shift) : 0;

                if (valueSelectionRenderer != NULL) {
                    if (activeSegmentLength > 0) {
                        valueSelectionRenderer->setValueSelection(activeSegmentStart, activeSegmentLength);
                    } else {
                        valueSelectionRenderer->clearValueSelection();
                    }
                }

                // Draw the item with the renderer, indicating if it's the last widget
                renderer->drawItem(text, buf, i == widgets.size() - 1);
                // Calculate the cursor column position for the active widget
                uint8_t endCol = renderer->getCursorCol();
                uint8_t offset = static_cast<uint8_t>(1 + widgets[i]->cursorOffset);
                cursorCol = endCol > offset ? endCol - offset : 0;
            }
        }
        buf[index < ITEM_DRAW_BUFFER_SIZE ? index : ITEM_DRAW_BUFFER_SIZE - 1] = '\0';

        if (valueSelectionRenderer != NULL) {
            if (MenuItem::isEditing() && activeSegmentLength > 0) {
                valueSelectionRenderer->setValueSelection(activeSegmentStart, activeSegmentLength);
            } else {
                valueSelectionRenderer->clearValueSelection();
            }
        }

        renderer->drawItem(text, buf);

        if (valueSelectionRenderer != NULL) {
            valueSelectionRenderer->clearValueSelection();
        }

        if (hasListWidget) {
            GraphicalIndicatorRenderer* indicatorRenderer =
                static_cast<GraphicalIndicatorRenderer*>(renderer->queryExtension(GraphicalIndicatorRenderer::extensionId()));
            if (indicatorRenderer != NULL) {
                indicatorRenderer->drawListIndicator();
            }
        }

        if (MenuItem::isEditing()) {
            renderer->moveCursor(cursorCol, renderer->getCursorRow());
        }
    }

    /**
     * @brief Processes a command for the active widget in the menu.
     *
     * This function handles the processing of commands for the active widget in
     * the menu. It first attempts to process the command using the active widget.
     * If the widget processes the command successfully, it redraws the menu and
     * returns true.
     *
     * If the renderer is in edit mode, it handles navigation commands (ENTER,
     * RIGHT, LEFT, BACK) to navigate through the widgets or exit edit mode.
     *
     * When the ENTER command is received while the renderer is in edit mode and
     * the active widget is not the last widget, it moves to the next widget. If
     * the active widget is the last widget, it exits edit mode and calls the
     * handleCommit function to commit the changes.
     *
     * If the renderer is not in edit mode and the ENTER command is received, it
     * sets the renderer to edit mode, redraws the menu, and draws a blinker.
     *
     * @param menu Pointer to the LcdMenu object.
     * @param command The command to be processed.
     * @return true if the command was processed successfully, false otherwise.
     */
    bool process(LcdMenu* menu, const unsigned char command) override {
        MenuRenderer* renderer = menu->getRenderer();
        if (MenuItem::isEditing()) {
            if (widgets[activeWidget]->process(menu, command)) {
                draw(renderer);
                return true;
            }
            switch (command) {
                case ENTER:
                    if (activeWidget < widgets.size() - 1) {
                        right(renderer);
                    } else {
                        back(renderer);
                    }
                    return true;
                case RIGHT:
                    right(renderer);
                    return true;
                case LEFT:
                    left(renderer);
                    return true;
                case BACK:
                    cancel(renderer);
                    return true;
                default:
                    return false;
            }
        }
        if (command == ENTER) {
            for (auto* w : widgets)
                if (w) w->startEdit();
            MenuItem::beginEdit();
            draw(renderer);
            renderer->drawBlinker();
            LOG(F("ItemWidget::enterEditMode"), this->text);
            return true;
        }
        return false;
    }

    void left(MenuRenderer* renderer) {
        if (activeWidget == 0) {
            activeWidget = widgets.size() - 1;
        } else {
            activeWidget--;
        }
        draw(renderer);
        LOG(F("ItemWidget::left"), activeWidget);
    }

    void right(MenuRenderer* renderer) {
        activeWidget = (activeWidget + 1) % widgets.size();
        draw(renderer);
        LOG(F("ItemWidget::right"), activeWidget);
    }

    void back(MenuRenderer* renderer) {
        MenuItem::endEdit();
        renderer->viewShift = 0;
        reset();
        handleCommit();
        renderer->clearBlinker();
        draw(renderer);
        LOG(F("ItemWidget::exitEditMode"), this->text);
    }

    void cancel(MenuRenderer* renderer) {
        MenuItem::endEdit();
        renderer->viewShift = 0;
        for (auto* w : widgets)
            if (w) w->cancelEdit();
        reset();
        renderer->clearBlinker();
        draw(renderer);
        LOG(F("ItemWidget::cancelEditMode"), this->text);
    }
};

#endif
