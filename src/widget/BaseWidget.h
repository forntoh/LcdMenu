// Creator: @ShishkinDmitriy

#ifndef BASE_WIDGET_H
#define BASE_WIDGET_H

#include <stdint.h>
#if defined(ARDUINO_ARCH_ESP32) || defined(ARDUINO_ARCH_ESP8266)
#else
#include "utils/printf.h"
#endif

#define ITEM_DRAW_BUFFER_SIZE 20

class LcdMenu;

/**
 * @class BaseWidget
 * @brief Base class for widget.
 */
class BaseWidget {
    template <typename T0, typename T1, typename T2>
    friend class ItemWidget;
    friend class BaseItemManyWidgets;
    friend class BaseItemSingleWidget;

  protected:
    const uint8_t cursorOffset = 0;

    BaseWidget(const uint8_t cursorOffset = 0) : cursorOffset(cursorOffset) {}

    /**
     * @brief Process a command decoded in 1 byte.
     * It can be a printable character or a control command like `ENTER` or `LEFT`.
     * Return value is used to determine operation was successful or ignored.
     * If the parent of item received that handle was ignored it can execute its own action on this command.
     * Thus, the item always has priority in processing; if it is ignored, it is delegated to the parent element.
     * Behaviour is very similar to Event Bubbling in JavaScript.
     * @param menu the owner menu of the item, can be used to retrieve required object, such as `MenuRenderer` or `MenuScreen`
     * @param command the character command, can be a printable character or a control command
     * @return true if command was successfully handled by item.
     */
    virtual bool process(LcdMenu* menu, unsigned char command) = 0;
    /**
     * @brief Draw the widget into specified buffer.
     *
     * @param buffer the buffer where widget will be drawn
     * @param start the index where to start drawing in the buffer
     */
    virtual uint8_t draw(char* buffer, const uint8_t start = 0) = 0;

  public:
    virtual ~BaseWidget() = default;
};

#endif