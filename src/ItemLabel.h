#pragma once

#include "MenuItem.h"

/**
 * @class ItemLabel
 * @brief An unselectable menu item used for titles or separators.
 */
class ItemLabel final : public MenuItem {
  public:
    explicit ItemLabel(const char* text) : MenuItem(text) {}

    bool isSelectable() const override { return false; }
};

/**
 * @brief Create a new unselectable label item.
 *
 * @param text The text to display for the item.
 * @return ItemLabel* The created item. Caller takes ownership of the returned pointer.
 */
inline ItemLabel* ITEM_LABEL(const char* text) { return new ItemLabel(text); }
