#pragma once

#ifndef ARDUINO_ARCH_ESP32
#ifndef ARDUINO_ARCH_ESP8266
#include <StandardCplusplus.h>
#endif
#endif

#include "LcdMenu.h"
#include "renderer/MenuRenderer.h"
#include "utils/constants.h"
#include <MenuItem.h>
#include <utils/utils.h>
#include <vector>

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
     * These items will be drawn on renderer.
     */
    std::vector<MenuItem*> items;
    /**
     * @brief Cursor position.
     *
     * ```
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
     * ```
     *
     * When `up` or `down` then this position will be moved over the items accordingly.
     * Always in range [`view`, `view` + `renderer.getMaxRows()` - 1].
     */
    uint8_t cursor = 0;
    /**
     * @brief First visible item's position in the menu array.
     *
     * ```
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
     * ```
     *
     * Is moved when `cursor` crosses the borders of visible area.
     * When number of items < `renderer.getMaxRows()` this index should be 0.
     * The size of the view is always the same and equals to `renderer.getMaxRows()`.
     */
    uint8_t view = 0;

  public:
    /**
     * Constructor
     */
    MenuScreen(std::vector<MenuItem*> items);
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
    /**
     * @brief Add a new item to the menu.
     */
    void addItem(MenuItem* item);
    /**
     * @brief Remove an item from the menu.
     */
    void removeItem(uint8_t position);

  protected:
    /**
     * @brief Move cursor to specified position.
     */
    void setCursor(MenuRenderer* renderer, uint8_t position);
    /**
     * @brief Draw the screen on screen.
     * @param renderer The renderer to use for drawing.
     */
    void draw(MenuRenderer* renderer);
    /**
     * @brief Sync indicators with the renderer.
     */
    void syncIndicators(uint8_t index, MenuRenderer* renderer);
    /**
     * @brief Process the command.
     * @return `true` if the command was processed, `false` otherwise.
     */
    bool process(LcdMenu* menu, const unsigned char command);
    /**
     * @brief Move cursor up.
     */
    void up(MenuRenderer* renderer);
    /**
     * @brief Move cursor down.
     */
    void down(MenuRenderer* renderer);
    /**
     * @brief Reset the screen to initial state.
     */
    void reset(MenuRenderer* renderer);
    /**
     * @brief Poll the screen for changes.
     * @param renderer The renderer to use for drawing.
     * @param pollInterval The interval to poll the screen.
     */
    void poll(MenuRenderer* renderer, uint16_t pollInterval);
};

#define MENU_SCREEN(screen, items, ...)           \
    extern std::vector<MenuItem*> items;          \
    extern MenuScreen* screen;                    \
    std::vector<MenuItem*> items = {__VA_ARGS__}; \
    MenuScreen* screen = new MenuScreen(items)
