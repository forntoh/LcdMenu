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
    const uint8_t cursorIcon;
    const uint8_t editCursorIcon;
    const uint8_t availableColumns;
    /**
     * @brief Calculates the available horizontal space for displaying content.
     *
     * This function computes the number of columns available for displaying content
     * on the display. It takes into account the presence of up and down arrows, which
     * occupy one column if either is present.
     *
     * @return The number of columns available for displaying content.
     */
    uint8_t getEffectiveCols() const override;

    /**
     * @brief Draws text on the display.
     *
     * This function draws text on the display, handling text truncation and shifting.
     * It takes into account the viewShift parameter to shift the text by a specified
     * number of columns.
     *
     * @param text The text to be drawn on the display.
     * @param col The column position to start drawing the text. This parameter will be updated to the new column position after drawing the text.
     * @param viewShift The number of columns to shift the text by.
     */
    inline void drawText(const char* text, uint8_t& col, uint8_t viewShift);

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
     * @param upArrow A pointer to an array of bytes representing the up arrow icon, default is ↑, if null, up arrow will not be displayed
     * @param downArrow A pointer to an array of bytes representing the down arrow icon, default is ↓, if null, down arrow will not be displayed
     */
    CharacterDisplayRenderer(
        CharacterDisplayInterface* display,
        const uint8_t maxCols,
        const uint8_t maxRows,
        const uint8_t cursorIcon = 0x7E,
        const uint8_t editCursorIcon = 0x7F,
        uint8_t* upArrow = new uint8_t[8]{0x04, 0x0E, 0x1F, 0x04, 0x04, 0x04, 0x04, 0x04},
        uint8_t* downArrow = new uint8_t[8]{0x04, 0x04, 0x04, 0x04, 0x04, 0x1F, 0x0E, 0x04});

    /**
     * @brief Initializes the renderer and creates custom characters on the display.
     */
    void begin() override;
    /**
     * @brief Draws a menu item on the character display.
     *
     * This function overrides the base class implementation to draw a menu item
     * on the character display. It handles appending a cursor to the text,
     * truncating the text if it's too long, padding the text with spaces,
     * appending an indicator to the text, and finally drawing the text on the display.
     *
     * @param text The text of the menu item to be drawn.
     * @param value The value of the menu item to be drawn.
     * @param paddWithBlanks A flag indicating whether to pad the text with spaces.
     */
    void drawItem(const char* text, const char* value, bool paddWithBlanks = true) override;
    void draw(uint8_t byte) override;
    void drawBlinker() override;
    void clearBlinker() override;
    void moveCursor(uint8_t cursorCol, uint8_t cursorRow) override;
};