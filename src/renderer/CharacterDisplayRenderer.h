#pragma once

#include "MenuRenderer.h"
#include "display/CharacterDisplayInterface.h"

/**
 * @class CharacterDisplayRenderer
 * @brief A renderer for character-based displays, extending the MenuRenderer class.
 *
 * This class provides functionality to render menus on character-based displays.
 * It supports custom icons for cursor and edit cursor, as well as up and down arrows
 * for navigation indicators.
 *
 * @details
 * The CharacterDisplayRenderer class is designed to work with displays that implement
 * the CharacterDisplayInterface. It provides methods to draw menu items, cursors, and
 * navigation indicators. The class also handles text truncation and cursor movement.
 *
 * @note
 * The class uses dynamic memory allocation for the upArrow and downArrow icons.
 */
class CharacterDisplayRenderer : public MenuRenderer {
  protected:
    uint8_t* upArrow;
    uint8_t* downArrow;
    uint8_t cursorIcon;
    uint8_t editCursorIcon;

  public:
    /**
     * @brief Constructor for CharacterDisplayRenderer.
     *        Initializes the renderer with the display, maximum columns, and maximum rows.
     *        Also sets the up and down arrow icons, cursor icons, and edit cursor icons.
     *
     * @param display A pointer to the CharacterDisplayInterface object.
     * @param maxCols The maximum number of columns on the display.
     * @param maxRows The maximum number of rows on the display.
     * @param upArrow A pointer to an array of bytes representing the up arrow icon, default is →
     * @param downArrow A pointer to an array of bytes representing the down arrow icon, default is ↓
     * @param cursorIcon A byte representing the cursor icon, default is →
     * @param editCursorIcon A byte representing the edit cursor icon, default is ←
     */
    CharacterDisplayRenderer(
        CharacterDisplayInterface* display,
        const uint8_t maxCols,
        const uint8_t maxRows,
        uint8_t* upArrow = new uint8_t[8]{0x04, 0x0E, 0x1F, 0x04, 0x04, 0x04, 0x04, 0x04},
        uint8_t* downArrow = new uint8_t[8]{0x04, 0x04, 0x04, 0x04, 0x04, 0x1F, 0x0E, 0x04},
        uint8_t cursorIcon = 0x7E,
        uint8_t editCursorIcon = 0x7F)
        : MenuRenderer(display, maxCols, maxRows), upArrow(upArrow), downArrow(downArrow), cursorIcon(cursorIcon), editCursorIcon(editCursorIcon) {}

    /**
     * @brief Initializes the renderer and creates custom characters on the display.
     */
    void begin() override {
        MenuRenderer::begin();
        static_cast<CharacterDisplayInterface*>(display)->createChar(0, upArrow);
    }

    /**
     * @brief Draws a menu item on the display.
     * @param itemIndex The index of the item in the menu.
     * @param screenRow The row on the screen where the item should be drawn.
     * @param text The text of the menu item.
     */
    void drawItem(uint8_t itemIndex, uint8_t screenRow, const char* text) override {
        MenuRenderer::drawItem(itemIndex, screenRow, text);
        // Truncate text if it's too long
        static char* buf = new char[maxCols];
        substring(text, 0, maxCols - (cursorIcon != 0 ? 2 : 1), buf);
        display->setCursor(cursorIcon != 0 ? 1 : 0, cursorRow);
        display->draw(buf);

        uint8_t spaces = constrain(maxCols - (cursorIcon != 0 ? 2 : 1) - strlen(text), 0, maxCols);
        for (uint8_t i = 0; i < spaces; i++) {
            display->draw(" ");
        }
        if (itemIndex == 0) {
            clearUpIndicator();
        } else if (itemIndex == itemCount - 1) {
            clearDownIndicator();
        }

        if (screenRow == 0 && itemIndex > 0) {
            drawUpIndicator();
        } else if (screenRow == maxRows - 1 && itemIndex < itemCount - 1) {
            drawDownIndicator();
        }
    }

    /**
     * @brief Draws a byte on the display.
     * @param byte The byte to be drawn.
     */
    void draw(uint8_t byte) override {
        display->draw(byte);
    }

    /**
     * @brief Draws the cursor on the display.
     */
    void drawCursor() override {
        if (cursorIcon != 0) {
            display->setCursor(cursorCol, cursorRow);
            display->draw(inEditMode ? editCursorIcon : cursorIcon);
        }
    }

    /**
     * @brief Draws the blinker on the display.
     */
    void drawBlinker() override {
        static_cast<CharacterDisplayInterface*>(display)->drawBlinker();
    }

    /**
     * @brief Clears the blinker from the display.
     */
    void clearBlinker() override {
        static_cast<CharacterDisplayInterface*>(display)->clearBlinker();
    }

    /**
     * @brief Moves the cursor to a specified position.
     * @param cursorCol The column position of the cursor.
     * @param cursorRow The row position of the cursor.
     */
    void moveCursor(uint8_t cursorCol, uint8_t cursorRow) override {
        MenuRenderer::moveCursor(cursorCol, cursorRow);
        display->setCursor(cursorCol, cursorRow);
    }

    /**
     * @brief Clears the cursor from the display.
     */
    void clearCursor() override {
        if (cursorIcon != 0) {
            display->setCursor(cursorCol, cursorRow);
            display->draw(" ");
        }
    }

    /**
     * @brief Draws the up arrow indicator on the display.
     */
    void drawUpIndicator() {
        if (upArrow != NULL) {
            display->setCursor(maxCols - 1, 0);
            display->draw(byte(0));
        }
    }

    /**
     * @brief Clears the up arrow indicator from the display.
     */
    void clearUpIndicator() {
        if (upArrow != NULL) {
            display->setCursor(maxCols - 1, 0);
            display->draw(" ");
        }
    }

    /**
     * @brief Draws the down arrow indicator on the display.
     */
    void drawDownIndicator() {
        if (downArrow != NULL) {
            display->setCursor(maxCols - 1, maxRows - 1);
            display->draw(byte((1)));
        }
    }

    /**
     * @brief Clears the down arrow indicator from the display.
     */
    void clearDownIndicator() {
        if (downArrow != NULL) {
            display->setCursor(maxCols - 1, maxRows - 1);
            display->draw(" ");
        }
    }
};