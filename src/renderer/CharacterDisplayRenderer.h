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

    /**
     * @brief Appends a cursor icon to the given text if the specified screen row is active.
     *
     * @param screenRow The row on the screen to check against the active row.
     * @param text The original text to which the cursor icon will be appended.
     * @param buf The buffer where the resulting text with the cursor icon will be stored.
     */
    void appendCursorToText(uint8_t screenRow, const char* text, char* buf) {
        uint8_t cursor;
        if (activeRow == screenRow) {
            cursor = inEditMode ? editCursorIcon : cursorIcon;
        } else {
            cursor = (cursorIcon != 0 || editCursorIcon != 0) ? ' ' : 0;
        }

        if (cursor != 0) {
            buf[0] = cursor;
            strcpy(buf + 1, text);
        } else {
            strcpy(buf, text);
        }
    }

    /**
     * @brief Appends an indicator to the provided text based on the item index and screen row.
     *
     * @param itemIndex The index of the item in the list.
     * @param screenRow The row on the screen where the text will be displayed.
     * @param text The original text to which the indicator may be appended.
     * @param buf The buffer where the resulting text with the indicator will be stored.
     */
    void appendIndicatorToText(uint8_t itemIndex, uint8_t screenRow, const char* text, char* buf) {
        uint8_t indicator = 0;
        if (screenRow == 0 && itemIndex > 0) {
            indicator = 1;
        } else if (screenRow == maxRows - 1 && itemIndex < itemCount - 1) {
            indicator = 2;
        }

        if (indicator != 0) {
            concat(text, indicator, buf);
        } else {
            strcpy(buf, text);
            if (upArrow != NULL || downArrow != NULL) {
                strcat(buf, " ");
            }
        }
    }

    /**
     * @brief Pads the given text with spaces to fit within the available length.
     *
     * This function takes a text string and pads it with spaces so that the total
     * length of the text fits within the available length of the display. If the
     * text is longer than the available length, no padding is added.
     *
     * @param text The input text to be padded.
     * @param itemIndex The index of the item (not used in the current implementation).
     * @param buf The buffer where the padded text will be stored. It should be large
     *            enough to hold the padded text.
     */
    void padText(const char* text, uint8_t itemIndex, char* buf) {
        uint8_t textLength = strlen(text);
        uint8_t spaces = (textLength > calculateAvailableLength()) ? 0 : calculateAvailableLength() - textLength;
        spaces = constrain(spaces, 0, maxCols);
        strcpy(buf, text);
        memset(buf + textLength, ' ', spaces);
        buf[textLength + spaces] = '\0';
    }

    /**
     * @brief Calculates the available length for display.
     *
     * This function computes the number of columns available for displaying content
     * on the character display. It takes into account the presence of up and down
     * arrows, which occupy one column if either is present.
     *
     * @return The number of columns available for displaying content.
     */
    uint8_t calculateAvailableLength() {
        return maxCols - (upArrow != NULL || downArrow != NULL ? 1 : 0);
    }

  public:
    /**
     * @brief Constructor for CharacterDisplayRenderer.
     *        Initializes the renderer with the display, maximum columns, and maximum rows.
     *        Also sets the up and down arrow icons, cursor icons, and edit cursor icons.
     *
     * @note slots 1 and 2 are reserved for up and down arrow icons.
     *       The available custom characters slots are 0 and 3 to 7.
     *
     * @param display A pointer to the CharacterDisplayInterface object.
     * @param maxCols The maximum number of columns on the display.
     * @param maxRows The maximum number of rows on the display.
     * @param cursorIcon A byte representing the cursor icon, default is →, if 0, cursor will not be displayed
     * @param editCursorIcon A byte representing the edit cursor icon, default is ←, if 0, edit cursor will not be displayed
     * @param upArrow A pointer to an array of bytes representing the up arrow icon, default is →, if null, up arrow will not be displayed
     * @param downArrow A pointer to an array of bytes representing the down arrow icon, default is ↓, if null, down arrow will not be displayed
     */
    CharacterDisplayRenderer(
        CharacterDisplayInterface* display,
        const uint8_t maxCols,
        const uint8_t maxRows,
        uint8_t cursorIcon = 0x7E,
        uint8_t editCursorIcon = 0x7F,
        uint8_t* upArrow = new uint8_t[8]{0x04, 0x0E, 0x1F, 0x04, 0x04, 0x04, 0x04, 0x04},
        uint8_t* downArrow = new uint8_t[8]{0x04, 0x04, 0x04, 0x04, 0x04, 0x1F, 0x0E, 0x04})
        : MenuRenderer(display, maxCols, maxRows), upArrow(upArrow), downArrow(downArrow), cursorIcon(cursorIcon), editCursorIcon(editCursorIcon) {}

    /**
     * @brief Initializes the renderer and creates custom characters on the display.
     */
    void begin() override {
        MenuRenderer::begin();
        static_cast<CharacterDisplayInterface*>(display)->createChar(1, upArrow);
        static_cast<CharacterDisplayInterface*>(display)->createChar(2, downArrow);
    }

    /**
     * @brief Draws a menu item on the character display.
     *
     * This function overrides the base class implementation to draw a menu item
     * on the character display. It handles appending a cursor to the text,
     * truncating the text if it's too long, padding the text with spaces,
     * appending an indicator to the text, and finally drawing the text on the display.
     *
     * @param itemIndex The index of the item in the menu.
     * @param screenRow The row on the screen where the item should be drawn.
     * @param text The text of the menu item to be drawn.
     */
    void drawItem(uint8_t itemIndex, uint8_t screenRow, const char* text) override {
        MenuRenderer::drawItem(itemIndex, screenRow, text);
        // Create a buffer to hold the final text
        char buf[maxCols + 1];

        // Append cursor to text
        appendCursorToText(screenRow, text, buf);

        // Truncate text if it's too long
        buf[calculateAvailableLength()] = '\0';
        uint8_t cursorCol = strlen(buf);

        // Pad text with spaces
        padText(buf, maxCols, buf);

        // Append indicator to text
        appendIndicatorToText(itemIndex, screenRow, buf, buf);

        // Draw text on display
        display->setCursor(0, screenRow);
        display->draw(buf);
        moveCursor(cursorCol, screenRow);
    }

    /**
     * @brief Draws a byte on the display.
     * @param byte The byte to be drawn.
     */
    void draw(uint8_t byte) override {
        display->draw(byte);
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
};