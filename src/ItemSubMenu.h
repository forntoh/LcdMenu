#ifndef ItemSubMenu_H
#define ItemSubMenu_H
#include "LcdMenu.h"
#include "MenuItem.h"

/**
 * @class ItemSubMenu
 * @brief Represents a submenu item in a menu.
 *
 * This class extends the MenuItem class and provides functionality to navigate
 * to a different screen when the item is selected.
 */
class ItemSubMenu : public MenuItem {
  private:
    MenuScreen*& screen;

  public:
    /**
     * @param text text to display for the item
     * @param screen the next screen to show
     */
    ItemSubMenu(const char* text, MenuScreen*& screen) : MenuItem(text), screen(screen) {}

  protected:
    bool process(LcdMenu* menu, const unsigned char command) {
        switch (command) {
            case ENTER:
                changeScreen(menu);
                return true;
            default:
                return false;
        }
    }
    void changeScreen(LcdMenu* menu) {
        LOG(F("ItemSubMenu::changeScreen"), text);
        screen->setParent(menu->getScreen());
        menu->setScreen(screen);
    }
};

#define ITEM_SUBMENU(...) (new ItemSubMenu(__VA_ARGS__))

#endif
