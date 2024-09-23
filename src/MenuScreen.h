#pragma once

#include "LcdMenu.h"
#include "display/DisplayInterface.h"
#include "utils/constants.h"
#include <MenuItem.h>
#include <utils/utils.h>

/**
 * @class MenuScreen
 * @brief Represents single screen with number of menu items.
 * Contains logic of navigating between items. Stores current cursor and view.
 */
class MenuScreen {
    friend LcdMenu;

  private:
    /**
     * @brief Previous screen.
     * When `BACK` command received this screen will be shown.
     */
    MenuScreen* parent = NULL;
    /**
     * @brief The menu items to be displayed on screen.
     * These items will be drawn on display.
     */
    MenuItem** items = NULL;
    /**
     * @brief Cursor position.
     *
     *     X X
     *     X X X X X
     * ┌───────────────┐
     * │   X X X       │
     * │   X X X X     │
     * │ > X X X X X X │ <── cursor
     * │   X X X X X   │
     * └───────────────┘
     *     X X X X
     *     X X X X X X
     *
     * When `up` or `down` then this position will be moved over the items accordingly.
     * Always in range [`view`, `view` + `display.getMaxRows()` - 1].
     */
    uint8_t cursor = 0;
    /**
     * @brief First visible item's position in the menu array.
     *
     *     X X
     *     X X X X X
     * ┌───────────────┐
     * │   X X X       │ <── view
     * │   X X X X     │
     * │ > X X X X X X │
     * │   X X X X X   │
     * └───────────────┘
     *     X X X X
     *     X X X X X X
     *
     * Is moved when `cursor` crosses the borders of visible area.
     * When number of items < `display.getMaxRows()` this index should be 0.
     * The size of the view is always the same and equals to `display.getMaxRows()`.
     */
    uint8_t view = 0;

  public:
    /**
     * Constructor
     */
    MenuScreen(MenuItem** items) : items(items) {}
    /**
     * @brief Set new parent screen.
     */
    void setParent(MenuScreen* parent);
    /**
     * @brief Get current cursor position.
     */
    uint8_t getCursor();
    /**
     * @brief Get a `MenuItem` at position.
     * @return `MenuItem` - item at `position`
     */
    MenuItem* getItemAt(uint8_t position);
    /**
     * @brief Get a `MenuItem` at position.
     * @return `MenuItem` - item at `position`
     */
    MenuItem* operator[](const uint8_t position);

  protected:
    /**
     * @brief Get the number of items in the menu.
     */
    uint8_t itemsCount();
    /**
     * @brief Move cursor to specified position.
     */
    void setCursor(DisplayInterface* display, uint8_t position);
    /**
     * @brief Draw the screen on display.
     */
    void draw(DisplayInterface* display);
    /**
     * @brief Process the command.
     * @return `true` if the command was processed, `false` otherwise.
     */
    bool process(LcdMenu* menu, const unsigned char command);
    /**
     * @brief Move cursor up.
     */
    void up(DisplayInterface* display);
    /**
     * @brief Move cursor down.
     */
    void down(DisplayInterface* display);
    /**
     * @brief Reset the screen to initial state.
     */
    void reset(DisplayInterface* display);
};

#define MENU_SCREEN(screen, items, ...)         \
    extern MenuItem* items[];                   \
    extern MenuScreen* screen;                  \
    MenuItem* items[] = {__VA_ARGS__, nullptr}; \
    MenuScreen* screen = new MenuScreen(items)
