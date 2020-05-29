#ifndef LcdMenu_H
#define LcdMenu_H
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

class LcdMenu {
   private:
    int cursorPosition = 0;
    unsigned int top = 0;
    unsigned int bottom;
    unsigned int maxRows;
    unsigned int maxCols;
    unsigned int lastIndex;
    LiquidCrystal_I2C* lcd;
    String* menuItems;
    byte downArrow[8] = {
        0b00100,  //   *
        0b00100,  //   *
        0b00100,  //   *
        0b00100,  //   *
        0b00100,  //   *
        0b10101,  // * * *
        0b01110,  //  ***
        0b00100   //   *
    };

    byte upArrow[8] = {
        0b00100,  //   *
        0b01110,  //  ***
        0b10101,  // * * *
        0b00100,  //   *
        0b00100,  //   *
        0b00100,  //   *
        0b00100,  //   *
        0b00100   //   *
    };

    byte menuCursor[8] = {
        0B01000,  //  *
        0B00100,  //   *
        0B00010,  //    *
        0B00001,  //     *
        0B00010,  //    *
        0B00100,  //   *
        0B01000,  //  *
        0B00000   //
    };
    void drawCursor();
    void drawMenu();

   public:
    LcdMenu(int maxRows, int maxCols, String menuItems[], int size);
    int getCursorPosition();
    void setupLCD(uint8_t lcd_Addr);
    void up();
    void down();
    void refresh();
};
#endif