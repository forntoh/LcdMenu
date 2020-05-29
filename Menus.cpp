#include "Menus.h"

Menus::Menus(int maxRows, int maxCols, String menuItems[], int size) {
    this->maxRows = maxRows;
    this->maxCols = maxCols;
    this->menuItems = menuItems;
    this->lastIndex = size - 1;
    this->bottom = maxRows - 1;
}

void Menus::setupLCD(uint8_t lcd_Addr) {
    lcd = new LiquidCrystal_I2C(lcd_Addr, maxCols, maxRows);
    lcd->init();
    lcd->backlight();
    lcd->clear();
    lcd->createChar(0, menuCursor);
    lcd->createChar(1, upArrow);
    lcd->createChar(2, downArrow);
    refresh();
}

int Menus::getCursorPosition() { return cursorPosition; }

void Menus::refresh() {
    drawMenu();
    drawCursor();
}

void Menus::drawCursor() {
    // Erases current cursor
    for (int x = 0; x < maxRows; x++) {
        lcd->setCursor(0, x);
        lcd->print(" ");
    }

    int line = constrain(cursorPosition - top, 0, maxRows - 1);
    lcd->setCursor(0, line);
    lcd->write(byte(0));
}

void Menus::drawMenu() {
    lcd->clear();

    for (int i = top; i <= bottom; i++) {
        lcd->setCursor(1, map(i, top, bottom, 0, maxRows - 1));
        lcd->print(menuItems[i]);
    }

    if (top == 0) {
        // Print the down arrow only
        lcd->setCursor(maxCols - 1, maxRows - 1);
        lcd->write(byte(2));
    } else if (top > 0 && bottom < lastIndex) {
        // Print the down arrow
        lcd->setCursor(maxCols - 1, maxRows - 1);
        lcd->write(byte(2));
        // Print the up arrow
        lcd->setCursor(maxCols - 1, 0);
        lcd->write(byte(1));
    } else if (bottom == lastIndex) {
        // Print the up arrow only
        lcd->setCursor(maxCols - 1, 0);
        lcd->write(byte(1));
    }
}

void Menus::up() {
    cursorPosition--;
    if (cursorPosition < 0) {
        cursorPosition = 0;
        return;
    }
    if (cursorPosition < top) {
        top--;
        bottom--;
        top = constrain(top, 0, lastIndex);
        bottom = constrain(bottom, 0, lastIndex);
    }
    refresh();
}

void Menus::down() {
    cursorPosition++;
    if (cursorPosition > lastIndex) {
        cursorPosition = lastIndex;
        return;
    }
    if (cursorPosition > bottom) {
        top++;
        bottom++;
        top = constrain(top, 0, lastIndex);
        bottom = constrain(bottom, 0, lastIndex);
    }
    refresh();
}