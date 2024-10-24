// Creator: @ShishkinDmitriy
#ifndef BASE_ITEM_MANY_WIDGETS_H
#define BASE_ITEM_MANY_WIDGETS_H

#include "LcdMenu.h"
#include "MenuItem.h"
#include "widget/BaseWidget.h"
#include <utils/utils.h>

class BaseItemManyWidgets : public MenuItem {
  protected:
    BaseWidget** widgets = nullptr;
    const uint8_t size = 0;
    uint8_t activeWidget = 0;

  public:
    BaseItemManyWidgets(const char* text, BaseWidget** widgets, const uint8_t size, uint8_t activeWidget = 0)
        : MenuItem(text), widgets(widgets), size(size), activeWidget(constrain(activeWidget, 0, size)) {}

    uint8_t getActiveWidget() const { return activeWidget; }
    void setActiveWidget(const uint8_t activeWidget) {
        if (activeWidget < size) {
            this->activeWidget = activeWidget;
        }
    }

    virtual ~BaseItemManyWidgets() {
        for (uint8_t i = 0; i < size; ++i)
            delete widgets[i];
        delete[] widgets;
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

        for (uint8_t i = 0; i < size; i++) {
            index += widgets[i]->draw(buf, index);
            if (i == activeWidget && renderer->isInEditMode()) {
                renderer->drawItem(text, buf, i == size - 1);
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
        if (widgets[activeWidget]->process(menu, command)) {
            draw(renderer);
            return true;
        }
        if (renderer->isInEditMode()) {
            switch (command) {
                case ENTER:
                    if (activeWidget < this->size - 1) {
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
            draw(renderer);
            renderer->drawBlinker();
            LOG(F("ItemWidget::enterEditMode"), this->text);
            return true;
        }
        return false;
    }

    void left(MenuRenderer* renderer) {
        if (activeWidget == 0) {
            activeWidget = this->size - 1;
        } else {
            activeWidget--;
        }
        draw(renderer);
        LOG(F("ItemWidget::left"), activeWidget);
    }

    void right(MenuRenderer* renderer) {
        activeWidget = (activeWidget + 1) % this->size;
        draw(renderer);
        LOG(F("ItemWidget::right"), activeWidget);
    }

    void back(MenuRenderer* renderer) {
        renderer->setEditMode(false);
        reset();
        handleCommit();
        renderer->clearBlinker();
        draw(renderer);
        LOG(F("ItemWidget::exitEditMode"), this->text);
    }
};

#endif