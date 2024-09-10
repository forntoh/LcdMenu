/**
 * ---
 *
 * # ItemSubMenu
 *
 * This item type indicates that the current item contains a sub menu.
 * The sub menu is opened when `enter()` is invoked.
 */

#ifndef MENU_SCREEN_H
#define MENU_SCREEN_H
#include "ActionHandler.h"
#include "MenuItem.h"
#include "interface/DisplayInterface.h"

class LcdMenu;

/**
 * Represent single screen with a number of menu items.
 */
class MenuScreen : public ActionHandler {
   private:
    /**
     * Backreference to the menu.
     */
    LcdMenu* menu = NULL;
    /**
     * Display used by menu.
     */
    DisplayInterface* display = NULL;
    /**
     * Backreference to the previous screen.
     */
    MenuScreen* parent = NULL;
    /**
     * The menu items to be displayed on screen.
     */
    MenuItem** items = NULL;
    /**
     * The number of menu items.
     */
    uint8_t itemsCount = 0;
    /**
     * Cursor position.
     */
    uint8_t cursor = 0;
    /**
     * First visible item's position in the menu array
     */
    uint8_t view = 0;
    /**
     * Number of visible items.
     */
    uint8_t viewSize = 0;

   public:
    /**
     * @param itemsCount the number of menu items
     * @param items the menu items to be displayed on screen
     */
    MenuScreen(uint8_t itemsCount, MenuItem* items[]);
    /**
     * Initialize the screen.
     */
    virtual void initialize(LcdMenu* menu, DisplayInterface* display);
    /**
     * Set new paren screen.
     */
    void setParent(MenuScreen* parent);
    /**
     * Get current cursor position.
     */
    uint8_t getCursor();
    /**
     * Move cursor to specified position.
     */
    void setCursor(uint8_t position);
    /**
     * Get a `MenuItem` at position.
     * @return `MenuItem` - item at `position`
     */
    MenuItem* getItemAt(uint8_t position);
    bool up() override;
    bool down() override;
    bool enter() override;
    bool back() override;
    bool left() override;
    bool right() override;
    bool backspace() override;
    bool type(const char c) override;
    bool clear() override;
    void draw();
    void reset();
};

#endif
