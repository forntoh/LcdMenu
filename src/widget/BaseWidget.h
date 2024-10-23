// Creator: @ShishkinDmitriy

#ifndef BASE_WIDGET_H
#define BASE_WIDGET_H

#ifndef ARDUINO_ARCH_ESP32
#ifndef ARDUINO_ARCH_ESP8266
#include "utils/printf.h"
#endif
#endif

#define ITEM_DRAW_BUFFER_SIZE 20

class LcdMenu;

/**
 * @class BaseWidget
 * @brief Base class for widget.
 */
class BaseWidget {
    template <typename... Ts>
    friend class ItemWidget;
    friend class BaseItemManyWidgets;

  protected:
    /**
     * @brief This member variable specifies the position where the cursor should be placed
     * within the widget's text. For example, if the text format is "%dms" (20ms) and the
     * user wants the cursor to be placed at the position of "%d", they would set
     * cursorOffset to 2. By default, the cursor is placed at the end of the resulting text.
     */
    const uint8_t cursorOffset;

    BaseWidget(const uint8_t cursorOffset = 0) : cursorOffset(cursorOffset) {}

    /**
     * @brief Process a command decoded in 1 byte.
     * It can be a printable character or a control command like `ENTER` or `LEFT`.
     * Return value is used to determine if the operation was successful or ignored.
     * If the item's handler ignores the command, the parent can execute its own action on this command.
     * Thus, the item always has priority in processing; if it ignores a command, the command is delegated to the parent element.
     * This behavior is similar to event delegation, where an event is handled by the most specific handler first,
     * and if not handled, it is passed up to more general handlers.
     * @param menu the owner menu of the item, can be used to retrieve required objects, such as `MenuRenderer` or `MenuScreen`
     * @param command the character command, can be a printable character or a control command
     * @return true if the command was successfully handled by the item.
     */
    virtual bool process(LcdMenu* menu, unsigned char command) = 0;
    /**
     * @brief Draw the widget into specified buffer.
     *
     * @param buffer the buffer where widget will be drawn
     * @param start the index where to start drawing in the buffer
     * @return the number of characters written into the buffer
     */
    virtual uint8_t draw(char* buffer, const uint8_t start = 0) = 0;

  public:
    virtual ~BaseWidget() = default;
};

#endif