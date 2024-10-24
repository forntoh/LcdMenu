// Creator: @ShishkinDmitriy
#ifndef BASE_ITEM_ZERO_WIDGET_H
#define BASE_ITEM_ZERO_WIDGET_H

#include "MenuItem.h"

/**
 * @class BaseItemZeroWidget
 * @brief A base class for menu items that either require custom commit handling or no commit handling.
 *
 * This class extends the MenuItem class and provides a framework for menu items
 * that either need no commit handling or need to handle commit actions in a specific way.
 * It requires derived classes to implement the `handleCommit` method for custom commit handling.
 *
 * @note This class is intended to be used as a base class for other menu items.
 *       It should not be instantiated directly.
 */
class BaseItemZeroWidget : public MenuItem {
  public:
    virtual ~BaseItemZeroWidget() = default;
    /**
     * @brief Construct a new BaseItemZeroWidget object.
     *
     * @param text The text to display for the menu item.
     */
    explicit BaseItemZeroWidget(const char* text) : MenuItem(text) {}

  protected:
    virtual void handleCommit(LcdMenu* menu) = 0;

    void draw(MenuRenderer* renderer) override {
        renderer->drawItem(text, nullptr);
    }

    bool process(LcdMenu* menu, const unsigned char command) override {
        if (command == ENTER) {
            handleCommit(menu);
            LOG(F("ItemWidget::enter"), text);
            return true;
        }
        return false;
    }
};

#endif