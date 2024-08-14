#ifndef DISPLAY_INTERFACE_H
#define DISPLAY_INTERFACE_H

#include <stdint.h>

class DisplayInterface {
   protected:
    uint8_t maxRows;
    uint8_t maxCols;
    uint8_t cursorPosition;
    MenuItem** currentMenuTable = NULL;
    bool isEditModeEnabled;

   public:
    uint8_t blinkerPosition;

    virtual void begin() = 0;

    virtual void update(MenuItem* menu[], uint8_t cursorPosition, uint8_t top,
                        uint8_t bottom) = 0;
    virtual void drawMenu() = 0;
    virtual void drawCursor() = 0;
#ifdef ItemInput_H
    virtual void resetBlinker() = 0;
    virtual bool drawChar(char c) = 0;
#endif
    virtual void clear() = 0;
    bool isAtTheStart() {
        byte menuType = currentMenuTable[cursorPosition - 1]->getType();
        return menuType == MENU_ITEM_MAIN_MENU_HEADER ||
               menuType == MENU_ITEM_SUB_MENU_HEADER;
    }

    bool isAtTheEnd() { return isAtTheEnd(cursorPosition); }

    bool isAtTheEnd(uint8_t position) {
        return currentMenuTable[position + 1]->getType() ==
               MENU_ITEM_END_OF_MENU;
    }

    virtual ~DisplayInterface() {}

    uint8_t getMaxRows() const { return maxRows; }
    uint8_t getMaxCols() const { return maxCols; }

    void setEditModeEnabled(bool isEnabled) { isEditModeEnabled = isEnabled; }
    bool getEditModeEnabled() { return isEditModeEnabled; }
};

#endif  // DISPLAY_INTERFACE_H
