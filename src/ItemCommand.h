#pragma once

#include "ItemWidget.h"

/**
 * @brief Create a new command item.
 *
 * @param text The text to display for the item.
 * @param callback The function to call when the item is selected.
 * @return MenuItem* The created item.
 */
inline MenuItem* ITEM_COMMAND(const char* text, void (*callback)()) {
    return new ItemWidget<>(text, callback);
}