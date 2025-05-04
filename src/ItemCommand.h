#pragma once

#include "BaseItemZeroWidget.h"

/**
 * @brief A menu item that executes a callback function when selected.
 *
 * This class extends the BaseItemZeroWidget class and provides a menu item
 * that executes a callback function when selected. The callback function is
 * provided as a std::function during construction.
 *
 * As a BaseItemZeroWidget, this item responds to selection events in the menu
 * system. When the user confirms the selection, handleCommit is triggered,
 * which executes the provided callback.
 */
class ItemCommand : public BaseItemZeroWidget {
  private:
    std::function<void()> callback;

  public:
    ItemCommand(const char* text, std::function<void()> callback) : BaseItemZeroWidget(text), callback(callback) {}

  protected:
    void handleCommit(LcdMenu* menu) override {
        if (callback) callback();
    }
};

/**
 * @brief Create a new command item.
 *
 * @param text The text to display for the item.
 * @param callback The function to call when the item is selected.
 * @return MenuItem* The created item. Caller takes ownership of the returned pointer.
 *
 * @example
 *   auto item = ITEM_COMMAND("Save", []() { save_data(); });
 */
inline MenuItem* ITEM_COMMAND(const char* text, std::function<void()> callback) {
    return new ItemCommand(text, callback);
}