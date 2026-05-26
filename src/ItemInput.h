#ifndef ItemInput_H
#define ItemInput_H

#include "LcdMenu.h"
#include "MenuItem.h"
#include "display/GraphicalDisplayInterface.h"
#include "renderer/GraphicalMenuItem.h"
#include "renderer/GraphicalValueSelectionRenderer.h"
#include <utils/lcd_menu_utils.h>

#include <string.h>

/**
 * @brief Item that allows user to input string information.
 *
 * ```
 * ┌────────────────────────────┐
 * │ > T E X T : V A L U E      │
 * └────────────────────────────┘
 * ```
 *
 * Additionally to `text` this item has string `value`.
 * Has internal `edit` state.
 * Value area is scrollable, see `view`.
 */
class ItemInput : public MenuItem, public GraphicalMenuItem {
  protected:
    /**
     * @brief String value of item.
     */
    char* value;
    /**
     * @brief The index of first visible character.
     *
     * ```
     *          visible area
     *        ┌───────────────┐
     * X X X X│X X X X █ X X X│X X
     *        └───────────────┘
     *   view--^
     * ```
     *
     * Is moved when `cursor` crosses the borders of visible area.
     * When length of `value` < `viewSize` this index should be 0.
     */
    uint8_t view = 0;
    /**
     * @brief Current index of the char to be edited.
     *
     * ```
     *          visible area
     *        ┌───────────────┐
     * X X X X│X X X X █ X X X│X X
     *        └───────────────┘
     *         cursor--^
     * ```
     *
     * When `left` or `right` then this position will be moved over the string accordingly.
     * When `type` then new character will be appended on this position.
     * When `backspace` then one character before will be removed.
     * Always in range [`view`, `view` + `viewSize` - 1].
     */
    uint8_t cursor;
    /**
     * The call back that will be executed when edit will be finished.
     * First parameter will be a `value` string.
     */
    fptrStr callback;

    inline GraphicalValueSelectionRenderer* getGraphicalValueSelectionRenderer(MenuRenderer* renderer) const {
        if (renderer == NULL) {
            return NULL;
        }
        return static_cast<GraphicalValueSelectionRenderer*>(renderer->queryExtension(GraphicalValueSelectionRenderer::extensionId()));
    }

  public:
    /**
     * Construct a new ItemInput object with an initial value.
     *
     * @param text The text to display for the item.
     * @param value The initial value for the input.
     * @param callback A reference to the callback function to be invoked when
     * the input is submitted.
     */
    ItemInput(const char* text, char* value, fptrStr callback)
        : MenuItem(text), value(value), callback(callback) {}
    /**
     * Construct a new ItemInput object with no initial value.
     *
     * @param text The text to display for the item.
     * @param callback A reference to the callback function to be invoked when
     * the input is submitted.
     */
    ItemInput(const char* text, fptrStr callback)
        : ItemInput(text, (char*)"", callback) {}
    /**
     * Get the current input value for this item.
     *
     * @return The current input value as a string.
     */
    char* getValue() { return value; }
    /**
     * Set the input value for this item.
     *
     * @note You need to call `LcdMenu::refresh` after this method to see the changes.
     *
     * @param value The new input value.
     *
     * @return true if the value was changed, false otherwise.
     */
    bool setValue(char* value) {
        if (this->value != value) {
            this->value = value;
            LOG(F("ItemInput::setValue"), value);
            return true;
        }
        return false;
    }

    uint8_t measureGraphicalValueWidth(GraphicalDisplayInterface* display) const override {
        return display == NULL ? 0 : display->getTextWidth(value);
    }

    bool useTightGraphicalSelectionBox() const override {
        return true;
    }

    const void* queryCapability(uint8_t capabilityId) const override {
        if (capabilityId == GraphicalMenuItem::capabilityId()) {
            return static_cast<const GraphicalMenuItem*>(this);
        }
        return MenuItem::queryCapability(capabilityId);
    }

    /**
     * Get the callback function for this item.
     *
     * @return The function pointer to the callback function.
     */
    fptrStr getCallbackStr() { return callback; }

  protected:
    void draw(MenuRenderer* renderer) override {
        GraphicalValueSelectionRenderer* selectionRenderer = getGraphicalValueSelectionRenderer(renderer);
        if (selectionRenderer != NULL) {
            char* graphicalValue = value;
            char* insertionBuffer = NULL;

            if (MenuItem::isEditing()) {
                renderer->viewShift = 0;
                uint8_t len = strlen(value);
                uint8_t selectionStart = cursor > len ? len : cursor;
                uint8_t selectionLength = 1;

                if (selectionStart >= len) {
                    insertionBuffer = new char[len + 2];
                    memcpy(insertionBuffer, value, len);
                    insertionBuffer[len] = ' ';
                    insertionBuffer[len + 1] = '\0';
                    graphicalValue = insertionBuffer;
                }

                selectionRenderer->setValueSelection(selectionStart, selectionLength);
            } else {
                selectionRenderer->clearValueSelection();
            }

            renderer->drawItem(text, graphicalValue);
            selectionRenderer->clearValueSelection();

            if (insertionBuffer != NULL) {
                delete[] insertionBuffer;
            }

            return;
        }

        const uint8_t viewSize = getViewSize(renderer);
        char* vbuf = new char[viewSize + 1];
        if (viewSize > 0) {
            substring(value, view, viewSize, vbuf);
        }
        vbuf[viewSize] = '\0';
        renderer->drawItem(text, vbuf);
        delete[] vbuf;
    }
    bool process(LcdMenu* menu, const unsigned char command) override {
        MenuRenderer* renderer = menu->getRenderer();
        if (MenuItem::isEditing()) {
            if (isprint(command)) {
                typeChar(renderer, command);
                return true;
            }
            switch (command) {
                case ENTER:
                    enter(renderer);
                    return true;
                case BACK:
                    back(renderer);
                    return true;
                case UP:
                    return true;
                case DOWN:
                    return true;
                case LEFT:
                    left(renderer);
                    return true;
                case RIGHT:
                    right(renderer);
                    return true;
                case BACKSPACE:
                    backspace(renderer);
                    return true;
                case CLEAR:
                    clear(renderer);
                    return true;
                default:
                    return false;
            }
        } else {
            switch (command) {
                case ENTER:
                    enter(renderer);
                    return true;
                default:
                    return false;
            }
        }
    }
    void enter(MenuRenderer* renderer) {
        bool graphicalSelection = getGraphicalValueSelectionRenderer(renderer) != NULL;

        // Move cursor to the latest editable index
        uint8_t length = strlen(value);
        if (graphicalSelection && length > 0) {
            cursor = length - 1;
        } else {
            cursor = length;
        }

        if (graphicalSelection) {
            view = 0;
            renderer->viewShift = 0;
        } else {
            // Move view if needed
            uint8_t viewSize = getViewSize(renderer);
            if (viewSize == 0) {
                viewSize = 1;
            }
            if (cursor > viewSize) {
                view = length - (viewSize - 1);
            }
        }

        // Redraw
        MenuItem::beginEdit();
        draw(renderer);
        if (!graphicalSelection) {
            renderer->drawBlinker();
        }
        // Log
        LOG(F("ItemInput::enterEditMode"), value);
    };
    void back(MenuRenderer* renderer) {
        renderer->clearBlinker();
        MenuItem::endEdit();

        renderer->viewShift = 0;

        if (callback != NULL) {
            callback(value);
        }

        // Move view to 0 and redraw before exit
        cursor = 0;
        view = 0;
        draw(renderer);

        // Log
        LOG(F("ItemInput::exitEditMode"), value);
    };
    void left(MenuRenderer* renderer) {
        if (cursor == 0) {
            return;
        }
        cursor--;

        if (getGraphicalValueSelectionRenderer(renderer) != NULL) {
            draw(renderer);
            LOG(F("ItemInput::left"), value);
            return;
        }

        uint8_t cursorCol = renderer->getCursorCol();
        if (view > 0 && cursor < view) {
            view--;
            draw(renderer);
        } else {
            cursorCol--;
        }
        renderer->moveCursor(cursorCol, renderer->getCursorRow());
        renderer->drawBlinker();
        // Log
        LOG(F("ItemInput::left"), value);
    };
    /**
     * @brief Moves the cursor to the right within the input value.
     */
    void right(MenuRenderer* renderer) {
        if (cursor == strlen(value)) {
            return;
        }
        cursor++;

        if (getGraphicalValueSelectionRenderer(renderer) != NULL) {
            draw(renderer);
            LOG(F("ItemInput::right"), value);
            return;
        }

        uint8_t viewSize = getViewSize(renderer);
        if (viewSize == 0) {
            viewSize = 1;
        }
        uint8_t cursorCol = renderer->getCursorCol();
        if (cursor > (view + viewSize - 1)) {
            view++;
            draw(renderer);
        } else {
            cursorCol++;
        }
        renderer->moveCursor(cursorCol, renderer->getCursorRow());
        renderer->drawBlinker();
        // Log
        LOG(F("ItemInput::right"), value);
    }
    /**
     * @brief Handles the backspace action for the input field.
     */
    void backspace(MenuRenderer* renderer) {
        if (strlen(value) == 0 || cursor == 0) {
            return;
        }
        remove(value, cursor - 1, 1);
        cursor--;

        if (getGraphicalValueSelectionRenderer(renderer) != NULL) {
            draw(renderer);
            LOG(F("ItemInput::backspace"), value);
            return;
        }

        uint8_t cursorCol = renderer->getCursorCol();
        if (view > 0) {
            view--;
        } else {
            cursorCol--;
        }
        draw(renderer);
        renderer->moveCursor(cursorCol, renderer->getCursorRow());
        renderer->drawBlinker();
        // Log
        LOG(F("ItemInput::backspace"), value);
    }
    /**
     * @brief Types a character into the current input value at the cursor position.
     *
     * This function inserts a character into the `value` string at the current cursor position.
     * If the cursor is within the current length of the string, the string is split and the character
     * is inserted in between. If the cursor is at the end of the string, the character is appended.
     * The cursor is then incremented, and the view is adjusted if necessary.
     * Finally, the renderer is updated and the blinker position is reset.
     *
     * @param renderer Pointer to the MenuRenderer object used for rendering.
     * @param character The character to be typed into the input value.
     */
    void typeChar(MenuRenderer* renderer, const unsigned char character) {
        uint8_t length = strlen(value);
        char* buf = new char[length + 2];
        if (cursor < length) {
            char start[length];
            char end[length];
            substring(value, 0, cursor, start);
            substring(value, cursor, length - cursor, end);
            concat(start, character, end, buf);
        } else {
            concat(value, character, buf);
        }
        delete[] value;
        value = buf;
        cursor++;

        if (getGraphicalValueSelectionRenderer(renderer) != NULL) {
            draw(renderer);
            LOG(F("ItemInput::typeChar"), character);
            return;
        }

        uint8_t viewSize = getViewSize(renderer);
        if (viewSize == 0) {
            viewSize = 1;
        }
        if (cursor > (view + viewSize - 1)) {
            view++;
        }
        draw(renderer);
        renderer->drawBlinker();
        // Log
        LOG(F("ItemInput::typeChar"), character);
    }
    /**
     * @brief Clear the value of the input field
     */
    void clear(MenuRenderer* renderer) {
        value[0] = '\0';
        cursor = 0;
        view = 0;
        draw(renderer);
        if (getGraphicalValueSelectionRenderer(renderer) == NULL) {
            renderer->drawBlinker();
        }
        // Log
        LOG(F("ItemInput::clear"), value);
    }
};

#define ITEM_INPUT(...) (new ItemInput(__VA_ARGS__))

#endif  // ITEM_INPUT_H
