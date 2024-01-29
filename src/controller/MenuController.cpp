#include "MenuController.h"

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
    printCmd(F("UP"), cursorPosition);
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
    printCmd(F("DOWN"), cursorPosition);
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

void MenuController::show() {
    enableUpdate = true;
    update();
}

void MenuController::updateTimer() {
    if (millis() == startTime + timeout) {
        displayOff();
    }
}
