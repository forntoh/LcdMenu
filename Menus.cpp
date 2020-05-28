#include "Menus.h"

Menus::Menus(int maxRows, int maxCols, String menuItems[]) {
    this->maxRows = maxRows;
    this->maxCols = maxCols;
    this->menuItems = menuItems;
    maxMenuPages = round(((sizeof(menuItems) / sizeof(String)) / maxRows) + .5);

    top = 0;
    bottom = maxRows;
}

void Menus::setupLCD(uint8_t lcd_Addr) {
    lcd = new LiquidCrystal_I2C(lcd_Addr, maxCols, maxRows);
    lcd->init();
    lcd->backlight();
    lcd->clear();
    lcd->createChar(0, menuCursor);
    lcd->createChar(1, upArrow);
    lcd->createChar(2, downArrow);
}

void Menus::drawCursor() {
    // Erases current cursor
    for (int x = 0; x < maxRows; x++) {
        lcd->setCursor(0, x);
        lcd->print(" ");
    }
    int line = constrain(cursorPosition, 0, maxRows - 1);
    lcd->setCursor(0, line);
    lcd->write(byte(0));
}

void Menus::drawMenu() {
    lcd->clear();

    for (int i = top; i < bottom; i++) {
        // int item = menuPage * maxRows + i;
        // if (item > sizeof(menuItems) / sizeof(String)) break;
        lcd->setCursor(1, i);
        lcd->print(menuItems[menuPage + i]);
    }

    if (menuPage == 0) {
        // Print the down arrow only
        lcd->setCursor(maxCols - 1, maxRows - 1);
        lcd->write(byte(2));
    } else if (menuPage > 0 && menuPage < maxMenuPages) {
        // Print the down arrow
        lcd->setCursor(maxCols - 1, maxRows - 1);
        lcd->write(byte(2));
        // Print the up arrow
        lcd->setCursor(maxCols - 1, 0);
        lcd->write(byte(1));
    } else if (menuPage == maxMenuPages) {
        // Print the up arrow only
        lcd->setCursor(maxCols - 1, 0);
        lcd->write(byte(1));
    }
}

void Menus::up() {
    cursorPosition -= 1;
    cursorPosition = constrain(cursorPosition, 0,
                               ((sizeof(menuItems) / sizeof(String)) - 1));

    if (cursorPosition % maxRows == 0) {
        menuPage--;
        menuPage = constrain(menuPage, 0, maxMenuPages);
    }

    drawMenu();
    drawCursor();
}

void Menus::down() {
    cursorPosition += 1;
    cursorPosition = constrain(cursorPosition, 0,
                               ((sizeof(menuItems) / sizeof(String)) - 1));

    if (cursorPosition % maxRows == 0) {
        menuPage++;
        menuPage = constrain(menuPage, 0, maxMenuPages);
    }

    drawMenu();
    drawCursor();
}