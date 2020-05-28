#pragma once
#ifndef Menus_H
#define Menus_H
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

class Menus {
   private:
    unsigned int top;
    unsigned int bottom;
    unsigned int maxRows;
    unsigned int maxCols;
    unsigned int cursorPosition = 0;
    unsigned int maxMenuPages;
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

   public:
    Menus(int maxRows, int maxCols, String menuItems[]);
    void setupLCD(uint8_t lcd_Addr);
    void drawCursor();
    void drawMenu();
    void up();
    void down();
};
#endif