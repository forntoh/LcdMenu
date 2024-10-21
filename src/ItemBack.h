#ifndef ITEM_BACK_H
#define ITEM_BACK_H

#include "BaseItemZeroWidget.h"
#include "LcdMenu.h"

class ItemBack : public BaseItemZeroWidget {

  public:
    /**
     * @brief A menu item that navigates back to the previous screen.
     *
     * The ItemBack class represents a menu item that, when selected,
     * navigates back to the previous screen in the menu system.
     *
     * @param text the text of the item (default is "<<")
     */
    ItemBack(const char* text = "<<") : BaseItemZeroWidget(text) {}

  protected:
    void handleCommit(LcdMenu* menu) override {
        menu->process(BACK);
    }
};

/**
 * @brief A menu item that navigates back to the previous screen.
 * @param text the text of the item (default is "<<")
 */
#define ITEM_BACK(...) \
    new ItemBack(__VA_ARGS__)

#endif  // ITEM_BACK_H
