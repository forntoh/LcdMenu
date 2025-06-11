#ifndef MENU_RENDERER_H
#define MENU_RENDERER_H

#include "display/DisplayInterface.h"
#include <Arduino.h>
#include <utils/utils.h>

/**
 * @class MenuRenderer
 * @brief Abstract base class for rendering a menu on a display.
 *
 * This class provides the basic structure and functionality for rendering a menu
 * on a display. It includes methods for drawing items, moving the cursor, and
 * handling edit mode.
 */
class MenuRenderer {
    friend class MenuScreen;

  protected:
    const uint8_t maxCols;
    const uint8_t maxRows;

    /**
     * @brief Flag indicating that there are hidden items above the current view.
     */
    bool hasHiddenItemsAbove = false;
    /**
     * @brief Flag indicating that there are hidden items below the current view.
     */
    bool hasHiddenItemsBelow = false;

    /**
     * @brief Flag indicating that the current item has focus.
     */
    bool hasFocus = false;

    uint8_t cursorCol;
    uint8_t cursorRow;

    bool inEditMode;

    unsigned long startTime = 0;

  public:
    /**
     * @brief Number of columns to shift the current item's view by.
     */
    uint8_t viewShift = 0;

    /**
     * Pointer to the display interface used for rendering.
     */
    DisplayInterface* display;

    /**
     * @brief Constructor for MenuRenderer.
     * @param display Pointer to the display interface.
     * @param maxCols Maximum number of columns in the display.
     * @param maxRows Maximum number of rows in the display.
     */
    MenuRenderer(DisplayInterface* display, uint8_t maxCols, uint8_t maxRows);

    /**
     * @brief Initializes the display and starts the timer.
     */
    virtual void begin();

    /**
     * @brief Function to draw a byte on the display.
     * @param byte The byte to be drawn.
     */
    virtual void draw(uint8_t byte) = 0;

    /**
     * @brief Draws an item on the display.
     * @param text Text of the item to be drawn.
     * @param value Value of the item to be drawn.
     * @param padWithBlanks Flag indicating whether to pad the text with spaces.
     */
    virtual void drawItem(const char* text, const char* value, bool padWithBlanks = true) = 0;

    /**
     * @brief Function to clear the blinker from the display.
     */
    virtual void clearBlinker() = 0;

    /**
     * @brief Function to draw the blinker on the display.
     */
    virtual void drawBlinker() = 0;

    /**
     * @brief Moves the cursor to a specified position.
     * @param cursorCol Column position to move the cursor to.
     * @param cursorRow Row position to move the cursor to.
     */
    virtual void moveCursor(uint8_t cursorCol, uint8_t cursorRow);

    /**
     * @brief Sets the edit mode for the menu.
     * @param inEditMode Flag indicating whether to enter or exit edit mode.
     */
    void setEditMode(bool inEditMode);

    /**
     * @brief Restarts the display timer and shows the display.
     */
    virtual void restartTimer();

    /**
     * @brief Updates the display timer and hides the display if the timeout is reached.
     */
    virtual void updateTimer() {
        if (millis() - startTime < DISPLAY_TIMEOUT) {
            return;
        }
        LOG(F("MenuRenderer::timeout"));
        display->hide();
    }

    /**
     * @brief Checks if the menu is in edit mode.
     * @return True if in edit mode, false otherwise.
     */
    bool isInEditMode() const;

    /**
     * @brief Gets the current column position of the cursor.
     * @return Current column position of the cursor.
     */
    uint8_t getCursorCol() const;

    /**
     * @brief Gets the current row position of the cursor.
     * @return Current row position of the cursor.
     */
    uint8_t getCursorRow() const;

    /**
     * @brief Gets the maximum number of rows in the display.
     * @return Maximum number of rows.
     */
    uint8_t getMaxRows() const;

    /**
     * @brief Gets the maximum number of columns in the display.
     * @return Maximum number of columns.
     */
    uint8_t getMaxCols() const;

    /**
     * @brief Calculates the available horizontal space for displaying content.
     * @return The horizontal space available for displaying content.
     */
    virtual uint8_t getEffectiveCols() const = 0;
};

#endif  // MENU_RENDERER_H