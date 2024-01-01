#ifndef LiquidCrystalMenu_H
#define LiquidCrystalMenu_H

#include <LiquidCrystal.h>

#include "../utils/utils.h"
#include "MenuController.h"

class LiquidCrystalMenu : public MenuController {
    using MenuController::MenuController;

   private:
    /**
     * Down arrow (↓)
     */
    byte downArrow[8];
    /**
     * Up arrow (↑)
     */
    byte upArrow[8];
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
    void drawCursor() override;
    /**
     * Draw the menu items with up and down indicators
     */
    void drawMenu() override;
#ifdef ItemInput_H
    /**
     * Calculate and set the new blinker position
     */
    void resetBlinker() override;
#endif
    /**
     * Turn on the display
     */
    void displayOn() override;
    /**
     * Turn off the display
     */
    void displayOff() override;

   public:
    /**
     * LCD Display
     */
    LiquidCrystal* lcd = NULL;
    /**
     * Call this function in `setup()` to initialize the LCD and the custom
     * characters used as up and down arrows
     * @param lcd_Addr address of the LCD on the I2C bus (default 0x27)
     * @param menu menu to display
     * @param timeout time to wait before putting display to sleep
     */
    void setupLcdWithMenu(uint8_t rs, uint8_t en, uint8_t d0, uint8_t d1,
                          uint8_t d2, uint8_t d3, MenuItem** menu,
                          uint16_t timeout = 10000);
    /*
     * Draw the menu items and cursor
     */
    void update() override;
#ifdef ItemInput_H
    /**
     * Draw a character on the display
     * used for `Input` type menu items.
     * @param c character to draw
     */
    void drawChar(char c) override;
#endif
    /**
     * Set the character used to visualize the cursor.
     * @param newIcon character to display
     */
    void setCursorIcon(uint8_t newIcon);
    /**
     * When you want to display any other content on the screen then
     * call this function then display your content, later call
     * `show()` to show the menu
     */
    void hide() override;
};
#endif