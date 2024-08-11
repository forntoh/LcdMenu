#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <utils/constants.h>

#include "DisplayInterface.h"

class LiquidCrystalI2CAdapter : public DisplayInterface {
   private:
    uint8_t downArrow[8];
    uint8_t upArrow[8];
    uint8_t top;
    uint8_t bottom;
    uint8_t blinkerPosition;
    unsigned long startTime = 0;

    void drawDownIndicator() { lcd.write(byte(1)); }
    void drawUpIndicator() { lcd.write(byte(0)); }

   public:
    uint8_t cursorIcon = CURSOR_ICON;
    uint8_t editCursorIcon = EDIT_CURSOR_ICON;
    uint16_t timeout = DISPLAY_TIMEOUT;
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
        lcd.write(isEditModeEnabled ? editCursorIcon : cursorIcon);
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

    void update(MenuItem* menu[], uint8_t cursorPosition,
                uint8_t blinkerPosition, uint8_t top, uint8_t bottom,
                bool isInEditMode) override {
        this->currentMenuTable = menu;
        this->top = top;
        this->bottom = bottom;
        this->cursorPosition = cursorPosition;
        this->blinkerPosition = blinkerPosition;
        this->isEditModeEnabled = isInEditMode;
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
        if (millis() == startTime + timeout) {
            lcd.noDisplay();
        }
    }
};
