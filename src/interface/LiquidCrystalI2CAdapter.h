#pragma once

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <utils/constants.h>

#include "DisplayInterface.h"

class LiquidCrystalI2CAdapter : public DisplayInterface {
   private:
    uint8_t downArrow[8];
    uint8_t upArrow[8];
    uint8_t top;
    uint8_t bottom;
    unsigned long startTime = 0;

   public:
    LiquidCrystal_I2C lcd;

    LiquidCrystalI2CAdapter(uint8_t lcd_Addr, uint8_t lcd_cols,
                            uint8_t lcd_rows)
        : lcd(lcd_Addr, lcd_cols, lcd_rows) {
        maxRows = lcd_rows;
        maxCols = lcd_cols;
        memcpy(upArrow, UP_ARROW, sizeof(UP_ARROW));
        memcpy(downArrow, DOWN_ARROW, sizeof(DOWN_ARROW));
    }

    void begin() override {
        lcd.init();
        lcd.clear();
        lcd.backlight();
        lcd.createChar(0, upArrow);
        lcd.createChar(1, downArrow);
        startTime = millis();
    }

    void clear() override { lcd.clear(); }
    
    void setEditModeEnabled(bool isEnabled) override {
        uint8_t previous = isEditModeEnabled;
        isEditModeEnabled = isEnabled;
        printCmd(F("setEditModeEnabled"));
        if (!previous && isEditModeEnabled || previous && !isEditModeEnabled) { // From false to true or From true to false
            drawCursor(false);
        }
    }

    void drawCursor(bool blink) override {
        //
        // Erases current cursor
        //
        for (uint8_t x = 0; x < maxRows; x++) {
            lcd.setCursor(0, x);
            lcd.print(" ");
        }
        //
        // draws a new cursor at [line]
        //
        uint8_t line = constrain(cursorPosition - top, 0, maxRows - 1);
        lcd.setCursor(0, line);
        lcd.write(isEditModeEnabled ? EDIT_CURSOR_ICON : CURSOR_ICON);
#ifdef ItemInput_H
        //
        // If cursor is at MENU_ITEM_INPUT enable blinking
        //
        if (blink) {
            resetBlinker();
            if (isEditModeEnabled) {
                lcd.blink();
                return;
            }
        }
#endif
        lcd.noBlink();
    }

    void update(uint8_t cursorPosition, uint8_t top, uint8_t bottom) override {
        this->top = top;
        this->bottom = bottom;
        this->cursorPosition = cursorPosition;
        this->startTime = millis();
        lcd.display();
        lcd.backlight();
    }

#ifdef ItemInput_H
    bool drawChar(char c) override {
        //
        // draw the character without updating the menu item
        //
        uint8_t line = constrain(cursorPosition - top, 0, maxRows - 1);
        lcd.setCursor(blinkerPosition, line);
        lcd.print(c);
        resetBlinker();
        // Log
        printCmd(F("DRAW-CHAR"), c);
        return true;
    }
    void resetBlinker() override {
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
        lcd.setCursor(blinkerPosition, cursorPosition - top);
    }
#endif

    void updateTimer() {
        if (millis() == startTime + DISPLAY_TIMEOUT) {
            printCmd(F("TIMEOUT"));
            lcd.noDisplay();
            lcd.noBacklight();
        }
    }
    
    Print* getPrint() override {
        return &lcd;
    }

    void setCursor(uint8_t col, uint8_t row) override {
        lcd.setCursor(col, row);
    }

    void drawDownIndicator(uint8_t col, uint8_t row) override { 
        lcd.setCursor(col, row);
        lcd.write(byte(1));
    }

    void drawUpIndicator(uint8_t col, uint8_t row) override { 
        lcd.setCursor(col, row);
        lcd.write(byte(0)); 
    }

};
