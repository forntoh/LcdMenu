#ifndef MenuController_H
#define MenuController_H

#include "../MenuItem.h"

class MenuController {
   protected:
    /**
     * Cursor position
     */
    uint8_t cursorPosition = 1;
    uint8_t previousCursorPosition = 1;
    /**
     * First visible item's position in the menu array
     */
    uint8_t top = 1;
    uint8_t previousTop = 1;
    /**
     * Edit mode
     */
    bool isEditModeEnabled = false;
    /**
     * Will prevent left and right movement when in edit mode and character
     * picker is active
     */
    bool isCharPickerActive = false;
    /**
     * Last visible item's position in the menu array
     */
    uint8_t bottom = 0;
    uint8_t previousBottom = 0;
    /**
     * Max rows for the meny
     */
    uint8_t maxRows;
    /**
     * Columns on the LCD Display
     */
    uint8_t maxCols;
    /**
     * Time when the timer started in milliseconds
     */
    unsigned long startTime = 0;
    /**
     * Column location of Blinker relative to input value
     */
    uint8_t blinkerPosition = 0;
    /**
     * Array of menu items
     */
    MenuItem** currentMenuTable = NULL;
    /**
     * Determines whether the screen should be updated after an action. Set it
     * to `false` when you want to display any other content on the screen then
     * set it back to `true` to show the menu.
     */
    bool enableUpdate = true;
    /**
     * Draws the cursor
     */
    virtual void drawCursor() = 0;
    /**
     * Draw the menu items with up and down indicators
     */
    virtual void drawMenu() = 0;
    /**
     * Check if the cursor is at the start of the menu items
     * @return true : `boolean` if it is at the start
     */
    boolean isAtTheStart() {
        byte menuType = currentMenuTable[cursorPosition - 1]->getType();
        return menuType == MENU_ITEM_MAIN_MENU_HEADER ||
               menuType == MENU_ITEM_SUB_MENU_HEADER;
    }
    /**
     * Check if the cursor is at the end of the menu items
     * @return true : `boolean` if it is at the end
     */
    boolean isAtTheEnd() {
        return currentMenuTable[cursorPosition + 1]->getType() ==
               MENU_ITEM_END_OF_MENU;
    }
    /**
     * Reset the display
     * @param isHistoryAvailable indicates if there is a previous position
     */
    void reset(boolean isHistoryAvailable) {
        if (isHistoryAvailable) {
            cursorPosition = previousCursorPosition;
            top = previousTop;
            bottom = previousBottom;
        } else {
            previousCursorPosition = cursorPosition;
            previousTop = top;
            previousBottom = bottom;

            cursorPosition = 1;
            top = 1;
            bottom = maxRows;
        }
        update();
    }
#ifdef ItemInput_H
    /**
     * Calculate and set the new blinker position
     */
    virtual void resetBlinker() = 0;
#endif

   public:
    /**
     * How long should the display stay on
     */
    uint16_t timeout = 10000;
    /**
     * Constructor for the LcdMenu class
     * @param maxRows rows on lcd display e.g. 4
     * @param maxCols columns on lcd display e.g. 20
     * @return new `LcdMenu` object
     */
    explicit MenuController(uint8_t maxRows, uint8_t maxCols)
        : bottom(maxRows), maxRows(maxRows), maxCols(maxCols) {}
    /*
     * Update the menu
     */
    virtual void update() = 0;
    /**
     * Reset the display
     */
    void resetMenu() { this->reset(false); }
    /**
     * Execute an "up press" on menu
     * When edit mode is enabled, this action is skipped
     */
    void up() {
        //
        // determine if cursor ia at start of menu items
        //
        if (isAtTheStart() || isEditModeEnabled) return;
        cursorPosition--;
        // Log
        printCmd(F("UP"));
        //
        // determine if cursor is at the top of the screen
        //
        if (cursorPosition < top) {
            //
            // scroll up once
            //
            top--;
            bottom--;
        }
        update();
    }
    /**
     * Execute a "down press" on menu
     * When edit mode is enabled, this action is skipped
     */
    void down() {
        //
        // determine if cursor has passed the end
        //
        if (isAtTheEnd() || isEditModeEnabled) return;
        cursorPosition++;
        // Log
        printCmd(F("DOWN"));
        //
        // determine if cursor is at the bottom of the screen
        //
        if (cursorPosition > bottom) {
            //
            // scroll down once
            //
            top++;
            bottom++;
        }
        update();
    }
    /**
     * Execute an "enter" action on menu.
     *
     * It does the following depending on the type of the current menu item:
     *
     * - Open a sub menu.
     * - Execute a callback action.
     * - Toggle the state of an item.
     */
    void enter() {
        // Log
        printCmd(F("ENTER"));
        //
        MenuItem* item = currentMenuTable[cursorPosition];
        //
        // determine the type of menu entry, then execute it
        //
        switch (item->getType()) {
            //
            // switch the menu to the selected sub menu
            //
            case MENU_ITEM_SUB_MENU: {
                //
                // check if there is a sub menu
                //
                if (item->getSubMenu() == NULL) return;
                currentMenuTable = item->getSubMenu();
                //
                // display the sub menu
                //
                reset(false);
                break;
            }
#ifdef ItemCommand_H
            //
            // execute the menu item's function
            //
            case MENU_ITEM_COMMAND: {
                //
                // execute the menu item's function
                //
                if (item->getCallback() != NULL) (item->getCallback())();
                //
                // display the menu again
                //
                update();
                break;
            }
#endif
#ifdef ItemToggle_H
            case MENU_ITEM_TOGGLE: {
                //
                // toggle the value of isOn
                //
                item->setIsOn(!item->isOn());
                //
                // execute the menu item's function
                //
                if (item->getCallbackInt() != NULL)
                    (item->getCallbackInt())(item->isOn());
                //
                // display the menu again
                //
                update();
                break;
            }
#endif
#ifdef ItemInput_H
            case MENU_ITEM_INPUT: {
                //
                // enter editmode
                //
                if (!isInEditMode()) {
                    isEditModeEnabled = true;
                    // blinker will be drawn
                    drawCursor();
                }
                break;
            }
#endif
#ifdef ItemList_H
            case MENU_ITEM_LIST: {
                //
                // execute the menu item's function
                //
                if (item->getCallbackInt() != NULL)
                    (item->getCallbackInt())(item->getItemIndex());
                break;
            }
#endif
        }
    }
    /**
     * Execute a "backpress" action on menu.
     *
     * Navigates up once.
     */
    void back() {
        // Log
        printCmd(F("BACK"));
        //
#ifdef ItemInput_H
        MenuItem* item = currentMenuTable[cursorPosition];
        //
        // Back action different when on ItemInput
        //
        if (item->getType() == MENU_ITEM_INPUT && isInEditMode()) {
            // Disable edit mode
            isEditModeEnabled = false;
            update();
            // Execute callback function
            if (item->getCallbackStr() != NULL)
                (item->getCallbackStr())(item->getValue());
            // Interrupt going back to parent menu
            return;
        }
#endif
        //
        // check if this is a sub menu, if so go back to its parent
        //
        if (isSubMenu()) {
            currentMenuTable = currentMenuTable[0]->getSubMenu();
            reset(true);
        }
    }
    /**
     * Execute a "left press" on menu
     *
     * *NB: Works only for `ItemInput` and `ItemList` types*
     *
     * Moves the cursor one step to the left.
     */
    void left() {
        //
        if (isInEditMode() && isCharPickerActive) return;
        //
        MenuItem* item = currentMenuTable[cursorPosition];
        //
        // get the type of the currently displayed menu
        //
#ifdef ItemList_H
        uint8_t previousIndex = item->getItemIndex();
#endif
        switch (item->getType()) {
#ifdef ItemList_H
            case MENU_ITEM_LIST: {
                item->setItemIndex(item->getItemIndex() - 1);
                if (previousIndex != item->getItemIndex()) update();
                // Log
                printCmd(F("LEFT"),
                         item->getItems()[item->getItemIndex()].c_str());
                break;
            }
#endif
#ifdef ItemInput_H
            case MENU_ITEM_INPUT: {
                blinkerPosition--;
                resetBlinker();
                // Log
                printCmd(F("LEFT"),
                         item->getValue()[blinkerPosition -
                                          (strlen(item->getText()) + 2)]);
                break;
            }
#endif
#ifdef ItemProgress_H
            case MENU_ITEM_PROGRESS: {
                if (isInEditMode()) {
                    item->decrement();
                    update();
                    // Log
                    printCmd(F("LEFT"), item->getValue());
                }
            }
#endif
            default:
                // Log
                printCmd(F("LEFT"));
                break;
        }
    }
    /**
     * Execute a "right press" on menu
     *
     * *NB: Works only for `ItemInput` and `ItemList` types*
     *
     * Moves the cursor one step to the right.
     */
    void right() {
        //
        // Is the menu in edit mode and is the character picker active?
        //
        if (isInEditMode() && isCharPickerActive) return;
        //
        MenuItem* item = currentMenuTable[cursorPosition];
        //
        // get the type of the currently displayed menu
        //
        switch (item->getType()) {
#ifdef ItemList_H
            case MENU_ITEM_LIST: {
                item->setItemIndex((item->getItemIndex() + 1) %
                                   item->getItemCount());
                // constrain(item->itemIndex + 1, 0, item->itemCount - 1);
                update();
                // Log
                printCmd(F("RIGHT"),
                         item->getItems()[item->getItemIndex()].c_str());
                break;
            }
#endif
#ifdef ItemInput_H
            case MENU_ITEM_INPUT: {
                blinkerPosition++;
                resetBlinker();
                // Log
                printCmd(F("RIGHT"),
                         item->getValue()[blinkerPosition -
                                          (strlen(item->getText()) + 2)]);
                break;
            }
#endif
#ifdef ItemProgress_H
            case MENU_ITEM_PROGRESS: {
                if (isInEditMode()) {
                    item->increment();
                    update();
                    // Log
                    printCmd(F("RIGHT"), item->getValue());
                }
                break;
            }
#endif
            default:
                // Log
                printCmd(F("RIGHT"));
                break;
        }
    }
#ifdef ItemInput_H
    /**
     * Execute a "backspace cmd" on menu
     *
     * *NB: Works only for `ItemInput` type*
     *
     * Removes the character at the current cursor position.
     */
    void backspace() {
        MenuItem* item = currentMenuTable[cursorPosition];
        //
        if (item->getType() != MENU_ITEM_INPUT) return;
        // Log
        printCmd(F("BACKSPACE"));
        //
        uint8_t p = blinkerPosition - (strlen(item->getText()) + 2) - 1;
        remove(item->getValue(), p, 1);

        blinkerPosition--;
        update();
    }
    /**
     * Display text at the cursor position
     * used for `Input` type menu items
     * @param character character to append
     */
    void type(char character) {
        MenuItem* item = currentMenuTable[cursorPosition];
        //
        if (item->getType() != MENU_ITEM_INPUT || !isEditModeEnabled) return;
        //
        // calculate lower and upper bound
        //
        uint8_t length = strlen(item->getValue());
        uint8_t lb = strlen(item->getText()) + 2;
        uint8_t ub = lb + length;
        ub = constrain(ub, lb, maxCols - 2);
        //
        // update text
        //
        if (blinkerPosition < ub) {
            static char start[10];
            static char end[10];
            static char* joined = new char[maxCols - lb];
            substring(item->getValue(), 0, blinkerPosition - lb, start);
            substring(item->getValue(), blinkerPosition + 1 - lb, length, end);
            concat(start, character, end, joined);
            item->setValue(joined);
        } else {
            static char* buf = new char[length + 2];
            concat(item->getValue(), character, buf);
            item->setValue(buf);
        }
        //
        isCharPickerActive = false;
        //
        // update blinker position
        //
        blinkerPosition++;
        //
        // repaint menu
        //
        update();
        // Log
        printCmd(F("TYPE-CHAR"), character);
    }
    /**
     * Draw a character on the display
     * used for `Input` type menu items.
     * @param c character to draw
     */
    virtual void drawChar(char c) = 0;
    /**
     * Clear the value of the input field
     */
    void clear() {
        MenuItem* item = currentMenuTable[cursorPosition];
        //
        if (item->getType() != MENU_ITEM_INPUT) return;
        // Log
        printCmd(F("CLEAR"));
        //
        // set the value
        //
        item->setValue(nullptr);
        //
        // update blinker position
        //
        blinkerPosition = 0;
        //
        // repaint menu
        //
        update();
    }
#endif
    /**
     * When you want to display any other content on the screen then
     * call this function then display your content, later call
     * `show()` to show the menu
     */
    virtual void hide() = 0;
    /**
     * Show the menu
     */
    void show() {
        enableUpdate = true;
        update();
    }
    /**
     * To know weather the menu is in edit mode or not
     * @return `bool` - isEditModeEnabled
     */
    bool isInEditMode() { return isEditModeEnabled; }
    /**
     * Get the current cursor position
     * @return `cursorPosition` e.g. 1, 2, 3...
     */
    uint8_t getCursorPosition() { return this->cursorPosition; }
    /**
     * Set the current cursor position
     * @param position
     */
    void setCursorPosition(uint8_t position) {
        this->cursorPosition = position;
    }
    /**
     * Check if currently displayed menu is a sub menu.
     */
    bool isSubMenu() {
        byte menuItemType = currentMenuTable[0]->getType();
        return menuItemType == MENU_ITEM_SUB_MENU_HEADER;
    }
    /**
     * Get a `MenuItem` at position
     * @return `MenuItem` - item at `position`
     */
    MenuItem* getItemAt(uint8_t position) { return currentMenuTable[position]; }
    /**
     * Get a `MenuItem` at position using operator function
     * e.g `menu[menu.getCursorPosition()]` will return the item at the current
     * cursor position
     * NB: This is relative positioning (i.e. if a submenu is currently being
     * displayed, menu[1] will return item 1 in the current menu)
     * @return `MenuItem` - item at `position`
     */
    MenuItem* operator[](const uint8_t position) {
        return currentMenuTable[position];
    }
};

#endif