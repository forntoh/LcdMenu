// Creator: @ShishkinDmitriy
#ifndef BASE_ITEM_SINGLE_WIDGET_H
#define BASE_ITEM_SINGLE_WIDGET_H

#include "MenuItem.h"
#include "widget/Widget.h"

/**
 * @class BaseItemSingleWidget
 * @brief A menu item that contains a single widget.
 *
 * This class represents a menu item that contains a single widget. It extends the MenuItem class
 * and provides functionality to draw the widget and handle user input.
 *
 * @note This class is abstract and cannot be instantiated directly. It requires the implementation
 * of the handleCommit() method.
 */
class BaseItemSingleWidget : public MenuItem {
  protected:
    BaseWidget* widget = nullptr;

  public:
    /**
     * @brief Construct a new BaseItemSingleWidget object.
     *
     * @param text The text to display for the menu item.
     * @param widget A pointer to the widget associated with this menu item.
     */
    explicit BaseItemSingleWidget(const char* text, BaseWidget* widget) : MenuItem(text), widget(widget) {}

  protected:
    virtual void handleCommit() = 0;

    /**
     * @brief Draws the menu item using the provided renderer.
     *
     * This function is responsible for rendering the menu item on the display.
     * It uses the provided `MenuRenderer` to draw the item text and widget content.
     * If the renderer is in edit mode, it adjusts the cursor position accordingly.
     *
     * @param renderer A pointer to the `MenuRenderer` object used for drawing the item.
     *
     * The drawing process involves the following steps:
     * 1. A buffer of size `ITEM_DRAW_BUFFER_SIZE` is created to hold the widget's drawn content.
     * 2. The widget's `draw` method is called to fill the buffer with its content.
     * 3. The renderer's `drawItem` method is called with the item text and the buffer content to render the item.
     * 4. If the renderer is in edit mode, the cursor position is adjusted by moving it to the left by the widget's `blinkerOffset` and an additional position.
     */
    void draw(MenuRenderer* renderer) override {
        char buf[ITEM_DRAW_BUFFER_SIZE];
        widget->draw(buf);
        renderer->drawItem(text, buf);

        if (renderer->isInEditMode()) {
            renderer->moveCursor(renderer->getCursorCol() - widget->blinkerOffset - 1, renderer->getCursorRow());
        }
    }

    /**
     * @brief Processes a command for the LcdMenu and updates the display accordingly.
     *
     * This function handles the processing of a command for the LcdMenu. It first attempts to
     * process the command using the widget. If the widget processes the command successfully,
     * it updates the display by calling the draw function and returns true.
     *
     * If the widget does not process the command and the renderer is in edit mode, it handles
     * specific commands (ENTER and BACK) to exit edit mode, commit changes, clear the blinker,
     * update the display, and log the action. If the command is not handled, it returns false.
     *
     * If the renderer is not in edit mode and the command is ENTER, it enters edit mode, updates
     * the display, draws the blinker, and logs the action. It then returns true.
     *
     * @param menu Pointer to the LcdMenu object.
     * @param command The command to be processed.
     * @return true if the command was processed and the display was updated, false otherwise.
     */
    bool process(LcdMenu* menu, const unsigned char command) override {
        MenuRenderer* renderer = menu->getRenderer();
        if (widget->process(menu, command)) {
            draw(renderer);
            return true;
        }
        if (renderer->isInEditMode()) {
            switch (command) {
                case ENTER:
                case BACK:
                    renderer->setEditMode(false);
                    handleCommit();
                    renderer->clearBlinker();
                    draw(renderer);
                    printLog(F("ItemWidget::exitEditMode"), this->text);
                    return true;
                default:
                    return false;
            }
        }
        if (command == ENTER) {
            renderer->setEditMode(true);
            draw(renderer);
            renderer->drawBlinker();
            printLog(F("ItemWidget::enterEditMode"), this->text);
            return true;
        }
        return false;
    }
};

#endif