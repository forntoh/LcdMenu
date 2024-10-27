#pragma once

#include "BaseItemZeroWidget.h"

/**
 * @class ItemSubMenu
 * @brief Represents a submenu item in a menu.
 *
 * This class extends the MenuItem class and provides functionality to navigate
 * to a different screen when the item is selected.
 */
class ItemSubMenu : public BaseItemZeroWidget {
  private:
    MenuScreen*& screen;

  public:
    /**
     * @param text text to display for the item
     * @param screen the next screen to show
     */
    ItemSubMenu(const char* text, MenuScreen*& screen) : BaseItemZeroWidget(text), screen(screen) {}

  protected:
    void handleCommit(LcdMenu* menu) override {
        LOG(F("ItemSubMenu::changeScreen"), text);
        screen->setParent(menu->getScreen());
        menu->setScreen(screen);
    }
};

/**
 * @brief Create a new submenu item.
 *
 * @param text The text to display for the item.
 * @param screen The screen to navigate to when the item is selected.
 * @return MenuItem* The created item. Caller takes ownership of the returned pointer.
 *
 * @example
 *   auto item = ITEM_SUBMENU("Settings", settingsScreen);
 */
inline MenuItem* ITEM_SUBMENU(const char* text, MenuScreen*& screen) {
    return new ItemSubMenu(text, screen);
}
