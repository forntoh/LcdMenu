#include <Arduino.h>
#include <LiquidCrystal.h>
#include <Wire.h>
#include <utils/constants.h>

#include "DisplayInterface.h"

class LiquidCrystalAdapter : public DisplayInterface {
   private:
    uint8_t downArrow[8];
    uint8_t upArrow[8];
    uint8_t top;
    uint8_t bottom;
    uint8_t cursorPosition;
    uint8_t blinkerPosition;
    unsigned long startTime = 0;

    void drawDownIndicator() { lcd.write(byte(1)); }
    void drawUpIndicator() { lcd.write(byte(0)); }

   public:
    LiquidCrystal lcd;

    LiquidCrystalAdapter(uint8_t rs, uint8_t en, uint8_t d0, uint8_t d1,
                         uint8_t d2, uint8_t d3, uint8_t lcd_cols,
                         uint8_t lcd_rows)
        : lcd(rs, en, d0, d1, d2, d3) {
        maxRows = lcd_rows;
        maxCols = lcd_cols;
        memcpy(upArrow, UP_ARROW, sizeof(UP_ARROW));
        memcpy(downArrow, DOWN_ARROW, sizeof(DOWN_ARROW));
    }

    void begin() override {
        lcd.begin(maxCols, maxRows);
        lcd.clear();
        lcd.createChar(0, upArrow);
        lcd.createChar(1, downArrow);
        startTime = millis();
    }

    void clear() override { lcd.clear(); }

    void drawCursor() override {
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
        MenuItem* item = currentMenuTable[cursorPosition];
        if (item->getType() == MENU_ITEM_INPUT) {
            resetBlinker();
            if (isEditModeEnabled) {
                lcd.blink();
                return;
            }
        }
#endif
        lcd.noBlink();
    }

    void update(MenuItem* menu[], uint8_t cursorPosition, uint8_t top,
                uint8_t bottom) override {
        this->currentMenuTable = menu;
        this->top = top;
        this->bottom = bottom;
        this->cursorPosition = cursorPosition;
        this->startTime = millis();
        lcd.display();
        drawMenu();
        drawCursor();
    }

    void drawMenu() override {
        lcd.clear();
        for (uint8_t i = top; i <= bottom; i++) {
            MenuItem* item = currentMenuTable[i];
            lcd.setCursor(1, map(i, top, bottom, 0, maxRows - 1));
            if (currentMenuTable[i]->getType() != MENU_ITEM_END_OF_MENU) {
                lcd.print(item->getText());
            }
            switch (item->getType()) {
#ifdef ItemToggle_H
                case MENU_ITEM_TOGGLE:
                    lcd.print(":");
                    lcd.print(item->isOn() ? item->getTextOn()
                                           : item->getTextOff());
                    break;
#endif
#if defined(ItemProgress_H) || defined(ItemInput_H)
                case MENU_ITEM_INPUT:
                case MENU_ITEM_PROGRESS:
                    static char* buf = new char[maxCols];
                    substring(item->getValue(), 0,
                              maxCols - strlen(item->getText()) - 2, buf);
                    lcd.print(":");
                    lcd.print(buf);
                    break;
#endif
#ifdef ItemList_H
                case MENU_ITEM_LIST:
                    static char* buff = new char[maxCols];
                    substring(item->getItems()[item->getItemIndex()].c_str(), 0,
                              maxCols - strlen(item->getText()) - 2, buff);
                    lcd.print(":");
                    lcd.print(buff);
                    break;
#endif
                default:
                    break;
            }
            if (currentMenuTable[i]->getType() == MENU_ITEM_END_OF_MENU) break;
        }
        if (top == 1 && !isAtTheEnd(bottom)) {
            lcd.setCursor(maxCols - 1, maxRows - 1);
            drawDownIndicator();
        } else if (!isAtTheStart() && !isAtTheEnd()) {
            lcd.setCursor(maxCols - 1, maxRows - 1);
            drawDownIndicator();
            lcd.setCursor(maxCols - 1, 0);
            drawUpIndicator();
        } else if (isAtTheEnd() && top != 1) {
            lcd.setCursor(maxCols - 1, 0);
            drawUpIndicator();
        }
    }

#ifdef ItemInput_H
    bool drawChar(char c) override {
        MenuItem* item = currentMenuTable[cursorPosition];
        //
        if (item->getType() != MENU_ITEM_INPUT || !isEditModeEnabled)
            return false;
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
            lcd.noDisplay();
        }
    }
};
