// Creator: @ShishkinDmitriy
#ifndef BASE_ITEM_MANY_WIDGETS_H
#define BASE_ITEM_MANY_WIDGETS_H

#include "LcdMenu.h"
#include "MenuItem.h"
#include "utils/std.h"
#include "utils/utils.h"
#include "widget/BaseWidget.h"
#include <vector>

class BaseItemManyWidgets : public MenuItem {
  protected:
    std::vector<BaseWidget*> widgets;
    int8_t activeWidget = -1;  // -1 means no active widget

  public:
    BaseItemManyWidgets(const char* text, std::vector<BaseWidget*> widgets, uint8_t activeWidget = 0)
        : MenuItem(text), widgets(widgets), activeWidget(constrain(activeWidget, 0, widgets.size())) {
        this->polling = true;
    }

    int8_t getActiveWidget() const { return activeWidget; }
    void setActiveWidget(const int8_t activeWidget) {
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
    void addWidget(BaseWidget* widget) {
        widgets.push_back(widget);
    }

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
            widgets.erase(widgets.begin() + index);
            if (activeWidget >= widgets.size()) {
                activeWidget = widgets.size() - 1;
            }
        }
    }

    virtual ~BaseItemManyWidgets() {
        for (auto widget : widgets) {
            delete widget;
        }
    }

  protected:
    virtual void handleCommit() = 0;
    /**
     * @brief Reset the active widget to no active widget.
     */
    void reset() { activeWidget = -1; }

    /**
     * @brief Draws the menu item using the provided renderer.
     *
     * This function is responsible for rendering the menu item and its associated widgets.
     * It iterates through each widget, drawing them into a buffer, and then uses the renderer
     * to display the combined result. If the renderer is in edit mode, it also handles the
     * cursor positioning for editing.
     *
     * @param renderer A pointer to the MenuRenderer object used for drawing the item.
     *
     * The function performs the following steps:
     * 1. Initializes a buffer to hold the drawn content and variables for indexing and cursor positioning.
     * 2. Iterates through each widget, drawing it into the buffer and updating the index.
     * 3. If the active widget is being edited, it draws the item using the renderer and calculates the cursor position.
     * 4. Draws the final item using the renderer.
     * 5. If in edit mode, moves the cursor to the appropriate position for editing.
     */
    void draw(MenuRenderer* renderer) override {
        char buf[ITEM_DRAW_BUFFER_SIZE];

        uint8_t index = 0;
        uint8_t cursorCol = 0;

        for (int8_t i = 0; i < widgets.size(); i++) {
            uint8_t widgetLength = widgets[i]->draw(buf, index);
            index += widgetLength;
            if (i == activeWidget && renderer->isInEditMode()) {
                if (!renderer->isBlinkerOn()) {
                    index -= widgetLength;
                    for (uint8_t j = 0; j < widgetLength; j++) {
                        buf[index] = ' ';  // clear the widget
                        index++;
                    }
                }
                // Calculate the available space for the widgets after the text
                size_t v_size = renderer->getEffectiveCols() - strlen(text) - 1;
                // Adjust the view shift to ensure the active widget is visible
                renderer->viewShift = index > v_size ? index - v_size : 0;
                // Draw the item with the renderer, indicating if it's the last widget
                renderer->drawItem(text, buf, i == widgets.size() - 1);
                // Calculate the cursor column position for the active widget
                cursorCol = renderer->getCursorCol() - 1 - widgets[i]->cursorOffset;
            }
        }
        renderer->drawItem(text, buf);

        if (renderer->isInEditMode()) {
            renderer->moveCursor(cursorCol, renderer->getCursorRow());
        }
    }

    /**
     * @brief Processes a command for the active widget in the menu.
     *
     * This function handles the processing of commands for the active widget in the menu.
     * It first attempts to process the command using the active widget. If the widget
     * processes the command successfully, it redraws the menu and returns true.
     *
     * If the renderer is in edit mode, it handles navigation commands (ENTER, RIGHT, LEFT, BACK)
     * to navigate through the widgets or exit edit mode.
     *
     * When the ENTER command is received while the renderer is in edit mode and the active widget
     * is not the last widget, it moves to the next widget. If the active widget is the last widget,
     * it exits edit mode and calls the handleCommit function to commit the changes.
     *
     * If the renderer is not in edit mode and the ENTER command is received, it sets the renderer
     * to edit mode, redraws the menu, and draws a blinker.
     *
     * @param menu Pointer to the LcdMenu object.
     * @param command The command to be processed.
     * @return true if the command was processed successfully, false otherwise.
     */
    bool process(LcdMenu* menu, const unsigned char command) override {
        MenuRenderer* renderer = menu->getRenderer();

        if (renderer->isInEditMode()) {
            renderer->resetBlinkerOn();
            if (activeWidget >= 0 && widgets[activeWidget]->process(menu, command)) {
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
                    back(renderer);
                    return true;
                default:
                    return false;
            }
        }
        if (command == ENTER) {
            renderer->setEditMode(true);
            if (activeWidget < 0) activeWidget = 0;  // activate first widget
            draw(renderer);
            renderer->drawBlinker();
            LOG(F("ItemWidget::enterEditMode"), this->text);
            return true;
        }
        return false;
    }

    void left(MenuRenderer* renderer) {
        if (activeWidget == 0) {
            activeWidget = widgets.size() - 1;  // wrap around
        } else if (activeWidget > 0) {
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
        renderer->setEditMode(false);
        renderer->viewShift = 0;
        reset();
        handleCommit();
        renderer->clearBlinker();
        draw(renderer);
        LOG(F("ItemWidget::exitEditMode"), this->text);
    }
};

#endif