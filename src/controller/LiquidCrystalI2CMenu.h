#ifndef LiquidCrystalI2CMenu_H
#define LiquidCrystalI2CMenu_H

#include <LiquidCrystal_I2C.h>

#include "../utils/utils.h"
#include "MenuController.h"

class LiquidCrystalI2CMenu : public MenuController {
    using MenuController::MenuController;

   private:
    /**
     * Down arrow (↓)
     */
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
    /**
     * Up arrow (↑)
     */
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
    /**
     * Cursor icon. Defaults to right arrow (→).
     */
    uint8_t cursorIcon = 0x7E;
    /**
     * The backlight state of the lcd
     */
    uint8_t backlightState = HIGH;
    /**
     * Draws the cursor
     */
    void drawCursor() override {
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
#ifdef ItemInput_H
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
#endif
        lcd->noBlink();
    }
    /**
     * Draw the menu items with up and down indicators
     */
    void drawMenu() override {
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
#ifdef ItemToggle_H
                case MENU_ITEM_TOGGLE:
                    //
                    // append textOn or textOff depending on the state
                    //
                    lcd->print(":");
                    lcd->print(item->isOn() ? item->getTextOn()
                                            : item->getTextOff());
                    break;
#endif
#ifdef ItemInput_H
                case MENU_ITEM_INPUT:
                    //
                    // append the value of the input
                    //
                    lcd->print(":");
                    lcd->print(
                        substring(item->getValue(), 0,
                                  maxCols - strlen(item->getText()) - 2));
                    break;
#endif
#ifdef ItemList_H
                case MENU_ITEM_LIST:
                    //
                    // append the value of the item at current list position
                    //
                    lcd->print(":");
                    lcd->print(item->getItems()[item->getItemIndex()].substring(
                        0, maxCols - strlen(item->getText()) - 2));
                    break;
#endif
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
#ifdef ItemInput_H
    /**
     * Calculate and set the new blinker position
     */
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
        lcd->setCursor(blinkerPosition, cursorPosition - top);
    }
#endif

   public:
    /**
     * LCD Display
     */
    LiquidCrystal_I2C* lcd = NULL;
    /**
     * Call this function in `setup()` to initialize the LCD and the custom
     * characters used as up and down arrows
     * @param lcd_Addr address of the LCD on the I2C bus (default 0x27)
     * @param menu menu to display
     * @param timeout time to wait before putting display to sleep
     */
    void setupLcdWithMenu(uint8_t lcd_Addr, MenuItem** menu,
                          uint16_t timeout = 10000) {
        lcd = new LiquidCrystal_I2C(lcd_Addr, maxCols, maxRows);
        lcd->init();
        lcd->backlight();
        lcd->clear();
        lcd->createChar(0, upArrow);
        lcd->createChar(1, downArrow);
        this->currentMenuTable = menu;
        this->startTime = millis();
        this->timeout = timeout;
        update();
    }
    /*
     * Draw the menu items and cursor
     */
    void update() override {
        if (!enableUpdate) return;
        lcd->display();
        lcd->setBacklight(backlightState);
        drawMenu();
        drawCursor();
        startTime = millis();
    }
#ifdef ItemInput_H
    /**
     * Draw a character on the display
     * used for `Input` type menu items.
     * @param c character to draw
     */
    void drawChar(char c) override {
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
    }
#endif
    /**
     * Set the character used to visualize the cursor.
     * @param newIcon character to display
     */
    void setCursorIcon(uint8_t newIcon) {
        cursorIcon = newIcon;
        drawCursor();
    }
    /**
     * When you want to display any other content on the screen then
     * call this function then display your content, later call
     * `show()` to show the menu
     */
    void hide() override {
        enableUpdate = false;
        lcd->clear();
    }
    /**
     * Set the Backlight state
     * @param state
     */
    void setBacklight(uint8_t state) {
        backlightState = state;
        update();
    }
};
#endif