/**
 * ---
 *
 * # ItemSubMenu
 *
 * This item type indicates that the current item contains a sub menu.
 * The sub menu is opened when `enter()` is invoked.
 */

#ifndef ItemSubMenu_H
#define ItemSubMenu_H
#include "LcdMenu.h"
#include "MenuItem.h"

class ItemSubMenu : public MenuItem {
  private:
    MenuScreen* screen;

  public:
    /**
     * @param text text to display for the item
     * @param screen the next screen to show
     */
    ItemSubMenu(const char* text, MenuScreen* screen) : MenuItem(text), screen(screen) {}

  protected:
    /**
     * Draw text of the item with asterisk (`*`) sign on the end.
     */
    void draw(DisplayInterface* display, uint8_t row) override {
        char* buf = new char[strlen(text) + 2];
        concat(text, '*', buf);
        display->drawItem(row, buf);
    }
    bool process(Context context) {
        switch (context.command) {
            case ENTER: return enter(context);
            default: return false;
        }
    }
    /**
     * Open next screen.
     */
    bool enter(Context context) {
        printCmd(F("Opening screen..."));
        screen->setParent(context.menu->getCurrentScreen());
        context.menu->setCurrentScreen(screen);
        return true;
    }
};

#define ITEM_SUBMENU(...) (new ItemSubMenu(__VA_ARGS__))

#endif
