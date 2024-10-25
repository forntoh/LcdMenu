#pragma once

#include "BaseItemZeroWidget.h"
#include "LcdMenu.h"

/**
 * @class ItemBack
 * @brief A menu item that navigates back to the previous screen.
 *
 * The ItemBack class represents a menu item that, when selected,
 * navigates back to the previous screen in the menu system.
 *
 * @details
 * This class inherits from the BaseItemZeroWidget class and overrides the
 * handleCommit method to handle the ENTER command by navigating back to the previous screen.
 */
class ItemBack : public BaseItemZeroWidget {
  public:
    /**
     * Construct a new ItemBack object.
     * @param text The text of the item.
     */
    ItemBack(const char* text) : BaseItemZeroWidget(text) {}

  protected:
    void handleCommit(LcdMenu* menu) override {
        menu->process(BACK);
    }
};

/**
 * @brief Create a new back item.
 *
 * @param text The text to display for the item (default is "..").
 * @return MenuItem* The created item. Caller takes ownership of the returned pointer.
 *
 * @example
 *   auto item = ITEM_BACK("Back");
 */
inline MenuItem* ITEM_BACK(const char* text = "..") { return new ItemBack(text); }
