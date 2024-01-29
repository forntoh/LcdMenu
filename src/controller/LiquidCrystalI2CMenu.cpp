#include "LiquidCrystalI2CMenu.h"

#include "../utils/constants.h"

void LiquidCrystalI2CMenu::setupLcdWithMenu(uint8_t lcd_Addr, MenuItem** menu,
                                            uint16_t timeout) {
    lcd = new LiquidCrystal_I2C(lcd_Addr, maxCols, maxRows);
    lcd->init();
    lcd->backlight();
    lcd->clear();
    memcpy(upArrow, UP_ARROW, sizeof(UP_ARROW));
    memcpy(downArrow, DOWN_ARROW, sizeof(DOWN_ARROW));
    lcd->createChar(0, upArrow);
    lcd->createChar(1, downArrow);
    this->currentMenuTable = menu;
    this->startTime = millis();
    this->timeout = timeout;
    update();
}

void LiquidCrystalI2CMenu::update() {
    if (!enableUpdate) return;
    displayOn();
    drawMenu();
    drawCursor();
    startTime = millis();
}

void LiquidCrystalI2CMenu::setCursorIcon(uint8_t newIcon) {
    cursorIcon = newIcon;
    drawCursor();
}

void LiquidCrystalI2CMenu::hide() {
    enableUpdate = false;
    lcd->clear();
}

void LiquidCrystalI2CMenu::setBacklight(uint8_t state) {
    backlightState = state;
    update();
}

void LiquidCrystalI2CMenu::displayOff() {
    lcd->noDisplay();
    lcd->noBacklight();
}

void LiquidCrystalI2CMenu::displayOn() {
    lcd->display();
    lcd->setBacklight(backlightState);
}

void LiquidCrystalI2CMenu::drawCursor() {
    //
    // Erases current cursor
    //
    for (uint8_t x = 0; x < maxRows; x++) {
        lcd->setCursor(0, x);
        lcd->print(" ");
    }
    //
    // draws a new cursor at [line]
    //
    uint8_t line = constrain(cursorPosition - top, 0, maxRows - 1);
    lcd->setCursor(0, line);
    lcd->write(cursorIcon);
    //
    // If cursor is at MENU_ITEM_INPUT enable blinking
    //
    MenuItem* item = currentMenuTable[cursorPosition];
    if (item->getType() == MENU_ITEM_INPUT) {
        resetBlinker();
        if (isEditModeEnabled) {
            lcd->blink();
            return;
        }
    }
    lcd->noBlink();
}

void LiquidCrystalI2CMenu::drawMenu() {
    lcd->clear();
    //
    // print the menu items
    //
    for (uint8_t i = top; i <= bottom; i++) {
        MenuItem* item = currentMenuTable[i];
        lcd->setCursor(1, map(i, top, bottom, 0, maxRows - 1));
        if (currentMenuTable[i]->getType() != MENU_ITEM_END_OF_MENU) {
            lcd->print(item->getText());
        }
        //
        // determine the type of item
        //
        switch (item->getType()) {
            case MENU_ITEM_TOGGLE:
                //
                // append textOn or textOff depending on the state
                //
                lcd->print(":");
                lcd->print(item->isOn() ? item->getTextOn()
                                        : item->getTextOff());
                break;
            case MENU_ITEM_INPUT:
            case MENU_ITEM_PROGRESS:
                //
                // append the value of the input
                //
                static char* buf = new char[maxCols];
                substring(item->getValue(), 0,
                          maxCols - strlen(item->getText()) - 2, buf);
                lcd->print(":");
                lcd->print(buf);
                break;
            case MENU_ITEM_LIST:
                //
                // append the value of the item at current list position
                //
                lcd->print(":");
                lcd->print(item->getItems()[item->getItemIndex()].substring(
                    0, maxCols - strlen(item->getText()) - 2));
                break;
            default:
                break;
        }
        // if we reached the end of menu, stop
        if (currentMenuTable[i]->getType() == MENU_ITEM_END_OF_MENU) break;
    }
    //
    // determine if cursor is at the top
    //
    if (top == 1) {
        //
        // Print the down arrow only
        //
        lcd->setCursor(maxCols - 1, maxRows - 1);
        lcd->write(byte(1));
    } else if (!isAtTheStart() && !isAtTheEnd()) {
        //
        // Print the down arrow
        //
        lcd->setCursor(maxCols - 1, maxRows - 1);
        lcd->write(byte(1));
        //
        // Print the up arrow
        //
        lcd->setCursor(maxCols - 1, 0);
        lcd->write(byte(0));
    } else if (isAtTheEnd()) {
        //
        // Print the up arrow only
        //
        lcd->setCursor(maxCols - 1, 0);
        lcd->write(byte(0));
    }
}

void LiquidCrystalI2CMenu::drawChar(char c) {
    MenuItem* item = currentMenuTable[cursorPosition];
    //
    if (item->getType() != MENU_ITEM_INPUT || !isEditModeEnabled) return;
    //
    // draw the character without updating the menu item
    //
    uint8_t line = constrain(cursorPosition - top, 0, maxRows - 1);
    lcd->setCursor(blinkerPosition, line);
    lcd->print(c);
    resetBlinker();
    //
    isCharPickerActive = true;
    // Log
    printCmd(F("DRAW-CHAR"), c);
}

void LiquidCrystalI2CMenu::resetBlinker() {
    //
    // calculate lower and upper bound
    //
    uint8_t lb = strlen(currentMenuTable[cursorPosition]->getText()) + 2;
    uint8_t ub = lb + strlen(currentMenuTable[cursorPosition]->getValue());
    ub = constrain(ub, lb, maxCols - 2);
    //
    // set cursor position
    //
    blinkerPosition = constrain(blinkerPosition, lb, ub);
    lcd->setCursor(blinkerPosition, cursorPosition - top);
}