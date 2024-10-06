/*
  MenuItem.h - MenuItem declaration and definition

  MIT License

  Copyright (c) 2020-2024 Forntoh Thomas

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

#ifndef MenuItem_H
#define MenuItem_H

#include "renderer/MenuRenderer.h"
#include "utils/constants.h"
#include <utils/utils.h>

class LcdMenu;
class MenuScreen;

/**
 * @class MenuItem
 * @brief Represent single item in menu.
 *
 * ```
 * ┌────────────────────────────┐
 * │ > T E X T                  │
 * └────────────────────────────┘
 * ```
 */
class MenuItem {
    friend MenuScreen;

  protected:
    const char* text = NULL;

  public:
    MenuItem(const char* text) : text(text) {}
    /**
     * @brief Get the text of the item
     * @return `String` - Item's text
     */
    const char* getText() {
        return text;
    }
    /**
     * @brief Set the text of the item
     * @note You need to call `LcdMenu::refresh` after this method to see the changes.
     * @param text text to display for the item
     */
    void setText(const char* text) {
        this->text = text;
    };

    // Destructor
    ~MenuItem() noexcept = default;

  protected:
    /**
     * @brief Process a command decoded in 1 byte.
     * It can be a printable character or a control command like `ENTER` or `LEFT`.
     * Return value is used to determine operation was successful or ignored.
     * If the parent of item received that handle was ignored it can execute it's own action on this command.
     * Thus, the item always has priority in processing; if it is ignored, it is delegated to the parent element.
     * Behaviour is very similar to Even Bubbling in JavaScript.
     * @param menu the owner menu of the item, can be used to retrieve required object, such as `DisplayInterface` or `MenuScreen`
     * @param command the character command, can be a printable character or a control command
     * @return true if command was successfully handled by item.
     */
    virtual bool process(LcdMenu* menu, const unsigned char command) {
        return false;
    };
    /**
     * @brief Draw this menu item on specified display on current row.
     * @param renderer The renderer to use for drawing.
     */
    const void draw(MenuRenderer* renderer) {
        draw(renderer, renderer->getActiveRow());
    };
    /**
     * @brief Draw this menu item on specified display on specified row.
     * @param renderer The renderer to use for drawing.
     * @param itemIndex The index of the item in the menu.
     * @param screenRow The row on the screen where the item should be drawn.
     */
    virtual void draw(MenuRenderer* renderer, uint8_t screenRow) {
        renderer->drawItem(screenRow, text);
    };
};

#define ITEM_BASIC(...) (new MenuItem(__VA_ARGS__))

#endif
