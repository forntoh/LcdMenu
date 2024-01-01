#include "MenuController.h"

#include "../utils/utils.h"

bool MenuController::isAtTheStart() {
    byte menuType = currentMenuTable[cursorPosition - 1]->getType();
    return menuType == MENU_ITEM_MAIN_MENU_HEADER ||
           menuType == MENU_ITEM_SUB_MENU_HEADER;
}

bool MenuController::isAtTheEnd() {
    return currentMenuTable[cursorPosition + 1]->getType() ==
           MENU_ITEM_END_OF_MENU;
}

bool MenuController::isSubMenu() {
    byte menuItemType = currentMenuTable[0]->getType();
    return menuItemType == MENU_ITEM_SUB_MENU_HEADER;
}

void MenuController::reset(boolean isHistoryAvailable) {
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

void MenuController::resetMenu() { this->reset(false); }

void MenuController::up() {
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

void MenuController::down() {
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

void MenuController::enter() {
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

void MenuController::back() {
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

void MenuController::left() {
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
            printCmd(F("LEFT"), item->getItems()[item->getItemIndex()].c_str());
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

void MenuController::right() {
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

void MenuController::backspace() {
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

void MenuController::type(char character) {
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
 * Clear the value of the input field
 */
void MenuController::clear() {
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

void MenuController::show() {
    enableUpdate = true;
    update();
}

void MenuController::updateTimer() {
    if (millis() == startTime + timeout) {
        displayOff();
    }
}
