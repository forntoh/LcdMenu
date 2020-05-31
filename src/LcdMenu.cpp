#include "LcdMenu.h"
//
// constructor for the LcdMenu class
//  Enter:  maxRows = rows on lcd display e.g. 4
//          maxCols = columns on lcd display e.g. 20
//
LcdMenu::LcdMenu(int maxRows, int maxCols) {
    this->maxRows = maxRows;
    this->maxCols = maxCols;
    this->bottom = maxRows;
}
//
// call this function in setup() to initialize the LCD and the custom characters
// used as up and down arrows
//  Enter:  lcd_Addr = address of the LCD display on the I2C bus (default 0x27)
//          menu     = menu to display
//
void LcdMenu::setupLcdWithMenu(uint8_t lcd_Addr, MenuItem *menu) {
    lcd = new LiquidCrystal_I2C(lcd_Addr, maxCols, maxRows);
    lcd->init();
    lcd->backlight();
    lcd->clear();
    lcd->createChar(0, upArrow);
    lcd->createChar(1, downArrow);
    currentMenuTable = menu;
    paint();
}
//
// call this function to draw the menu items and cursor
//
void LcdMenu::paint() {
    drawMenu();
    drawCursor();
}
//
// call this function to reset the display
//
void LcdMenu::reset() {
    cursorPosition = 1;
    top = 1;
    bottom = maxRows;
    paint();
}
//
// this function checks if the cursor is at the start of the menu items
//
boolean LcdMenu::isAtTheStart() {
    byte menuType = currentMenuTable[cursorPosition - 1].getType();
    return menuType == MENU_ITEM_MAIN_MENU_HEADER || menuType == MENU_ITEM_SUB_MENU_HEADER;
}
//
// this function checks if the cursor is at the end of the menu items
//
boolean LcdMenu::isAtTheEnd() {
    return currentMenuTable[cursorPosition + 1].getType() == MENU_ITEM_END_OF_MENU;
}
//
// this function draws the cursor
//
void LcdMenu::drawCursor() {
    //
    // Erases current cursor
    //
    for (int x = 0; x < maxRows; x++) {
        lcd->setCursor(0, x);
        lcd->print(" ");
    }
    //
    // draws a new cursor at [line]
    //
    int line = constrain(cursorPosition - top, 0, maxRows - 1);
    lcd->setCursor(0, line);
    lcd->write(0x7E);
    //
    // If cursor is at MENU_ITEM_INPUT enable blinking
    //
    if (currentMenuTable[cursorPosition].getType() == MENU_ITEM_INPUT)
        lcd->blink();
    else
        lcd->noBlink();
}
//
// this function draws the menu items with up and down indicators
//
void LcdMenu::drawMenu() {
    lcd->clear();
    //
    // print the menu items
    //
    for (int i = top; i <= bottom; i++) {
        MenuItem *item = &currentMenuTable[i];
        lcd->setCursor(1, map(i, top, bottom, 0, maxRows - 1));
        lcd->print(item->getText());

        switch (item->getType()) {
            case MENU_ITEM_TOGGLE:
                lcd->print(": ");
                lcd->print(item->isOn ? item->textOn : item->textOff);
                break;
            case MENU_ITEM_INPUT:
                lcd->print(": ");
                lcd->print(item->value.substring(
                    0, maxCols - ((String)item->getText()).length() - 3));
                break;
            default:
                break;
        }
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
//
// call this function to execute an "up press"
//
void LcdMenu::up() {
    //
    // determine if cursor ia at start of menu items
    //
    if (isAtTheStart()) return;
    cursorPosition--;
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
    paint();
}
//
// call this function to execute a "down press"
//
void LcdMenu::down() {
    //
    // determine if cursor has passed the end
    //
    if (isAtTheEnd()) return;
    cursorPosition++;
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
    paint();
}
//
// call this function to execute a "select"
//
void LcdMenu::select() {
    MenuItem *item = &currentMenuTable[cursorPosition];
    //
    // determine the type of menu entry, then execute it
    //
    switch (item->getType()) {
        //
        // switch the menu to the selected sub menu
        //
        case MENU_ITEM_SUB_MENU: {
            currentMenuTable = item->getSubMenu();
            //
            // display the parent menu
            //
            reset();
            break;
        }
        //
        // execute the menu item's function
        //
        case MENU_ITEM_COMMAND: {
            //
            // execute the menu item's function
            //
            (item->getCallback())();
            //
            // display the menu again
            //
            paint();
            break;
        }
        case MENU_ITEM_TOGGLE: {
            //
            // toggle the value of isOn
            //
            item->isOn = !item->isOn;
            //
            // execute the menu item's function
            //
            (item->getCallback())();
            //
            // display the menu again
            //
            paint();
            break;
        }
        case MENU_ITEM_INPUT: {
            break;
        }
    }
}
//
// call this function to execute a "backpress"
//
void LcdMenu::back() {
    //
    // get the type of the currently displayed menu
    //
    byte menuItemType = currentMenuTable[0].getType();
    //
    // check if this is a sub menu, if so go back to its parent
    //
    if (menuItemType == MENU_ITEM_SUB_MENU_HEADER) {
        currentMenuTable = currentMenuTable[0].getSubMenu();
        reset();
    }
}
//
// display text at the cursor position
//  text: String        = text to display
//  isPassword: boolean = determines wether text should be hidden or not
//
void LcdMenu::setText(String text, boolean isPassword) {
    //
    // get the type of the currently displayed menu
    //
    byte menuItemType = currentMenuTable[cursorPosition].getType();
    //
    // check if this is input menu type, if so print text
    //
    if (menuItemType == MENU_ITEM_INPUT) {
        //
        // clear the line where the cursor is
        //
        lcd->noBlink();
        for (int i = text.length() + 1; i < maxCols; i++) {
            lcd->setCursor(i, cursorPosition - 1);
            lcd->write(0x10);
        }
        lcd->blink();
        lcd->setCursor(1, cursorPosition - 1);
        //
        // print the text
        //
        for (int i = 0; i < constrain(text.length(), 0, maxCols - 2); i++) {
            lcd->setCursor(i + 1, cursorPosition - 1);
            if (isPassword) {
                lcd->print('*');
            } else {
                lcd->print(text[i]);
            }
        }
    }
}