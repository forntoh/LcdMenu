#pragma once

#include "utils/custom_printf.h"

#include "BaseItemZeroWidget.h"

/**
 * @class ItemValue
 * @brief A menu item that displays a value.
 *
 * This class extends the BaseItemZeroWidget class and provides a menu item
 * that displays a value. The value is provided as a reference during construction
 * and is displayed using the provided format string.
 */
template <typename T>
class ItemValue : public BaseItemZeroWidget {
  private:
    T& value;
    const char* format;

  public:
    ItemValue(const char* text, T& value, const char* format) : BaseItemZeroWidget(text), value(value), format(format) {
        this->polling = true;
    }

  protected:
    void handleCommit(LcdMenu* menu) override {}

    void draw(MenuRenderer* renderer) override {
        char buffer[ITEM_DRAW_BUFFER_SIZE];
        snprintf(buffer, ITEM_DRAW_BUFFER_SIZE, format, value);
        renderer->drawItem(text, buffer);
    }
};

/**
 * @brief Create a new item that displays a value.
 * @note If you want to display a value that changes over time, use this item and
 *       call `LcdMenu::poll` in the loop function.
 *
 * @tparam T the type of the value to display
 * @param text the text to display for the item
 * @param value the value to display
 * @param format the format string to use when displaying the value
 * @return MenuItem* the created item
 *
 * @example
 *   int temp = 25;
 *   auto item = ITEM_VALUE("Temperature", temp, "%d°C");
 */
template <typename T>
inline MenuItem* ITEM_VALUE(
    const char* text,
    T& value,
    const char* format = "%s") {
    return new ItemValue<T>(text, value, format);
}