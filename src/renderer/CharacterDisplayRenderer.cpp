#include "CharacterDisplayRenderer.h"

CharacterDisplayRenderer::CharacterDisplayRenderer(
    CharacterDisplayInterface* display,
    const uint8_t maxCols,
    const uint8_t maxRows,
    const uint8_t cursorIcon,
    const uint8_t editCursorIcon,
    uint8_t* upArrow,
    uint8_t* downArrow)
    : MenuRenderer(display, maxCols, maxRows),
      upArrow(upArrow),
      downArrow(downArrow),
      cursorIcon(cursorIcon),
      editCursorIcon(editCursorIcon),
      availableColumns(maxCols - (upArrow != NULL || downArrow != NULL ? 1 : 0)) {}

void CharacterDisplayRenderer::begin() {
    MenuRenderer::begin();
    if (upArrow != NULL && downArrow != NULL) {
        static_cast<CharacterDisplayInterface*>(display)->createChar(0, upArrow);
        static_cast<CharacterDisplayInterface*>(display)->createChar(1, downArrow);
    }
}

void CharacterDisplayRenderer::drawItem(const char* text, const char* value, bool padWithBlanks) {
    uint8_t cursorCol = 0;
    display->setCursor(cursorCol, cursorRow);

    // Draw cursor or empty space based on focus and edit mode
    if (cursorIcon != 0 || editCursorIcon != 0) {
        display->draw(hasFocus ? (inEditMode ? editCursorIcon : cursorIcon) : ' ');
        cursorCol++;
    }

    // Draw text
    drawText(text, cursorCol, viewShift);

    // Draw colon separator if value is present and within bounds
    if (value && cursorCol < availableColumns && (!hasFocus || viewShift < strlen(text) + 1)) {
        display->draw(':');
        cursorCol++;
    }

    // Draw value if present
    if (value) {
        uint8_t textLen = strlen(text);
        uint8_t valueViewShift = (viewShift > textLen) ? viewShift - textLen - 1 : 0;
        drawText(value, cursorCol, valueViewShift);
    }

    uint8_t cursorColEnd = cursorCol;

    // Fill remaining space with whitespace only when padWithBlanks is true
    if (padWithBlanks) {
        for (; cursorCol < availableColumns; cursorCol++) {
            display->draw(' ');
        }
    }

    // Draw up and down arrows if present
    if (upArrow && downArrow) {
        uint8_t indicator = hasHiddenItemsAbove ? 0 : (hasHiddenItemsBelow ? 1 : ' ');
        display->setCursor(maxCols - 1, cursorRow);
        display->draw(indicator);
    }

    // Move cursor to the end position if focused
    if (hasFocus) moveCursor(cursorColEnd, cursorRow);
}

void CharacterDisplayRenderer::drawText(const char* text, uint8_t& col, uint8_t shift) {
    // Pointer to the current character in the text
    const char* textPtr = text;

    // If the renderer has focus, adjust the text pointer based on the shift value
    if (hasFocus) {
        uint8_t textLen = strlen(text);
        // Move the text pointer forward by 'shift' characters, if within bounds
        textPtr = (shift < textLen) ? textPtr + shift : NULL;
    }

    // Draw characters from the text until we reach the end of the available columns or the end of the text
    while (col < availableColumns && textPtr && *textPtr) {
        display->draw(*textPtr++);  // Draw the current character and move to the next
        col++;                      // Move to the next column
    }
}

void CharacterDisplayRenderer::draw(uint8_t byte) {
    display->draw(byte);
}

void CharacterDisplayRenderer::drawBlinker() {
    //    static_cast<CharacterDisplayInterface*>(display)->drawBlinker();
}

void CharacterDisplayRenderer::clearBlinker() {
    //    static_cast<CharacterDisplayInterface*>(display)->clearBlinker();
}

constexpr uint32_t BLINK_ON_MS  = 600;
constexpr uint32_t BLINK_OFF_MS = 200;

bool CharacterDisplayRenderer::isBlinkerOn() {
    uint32_t currentMillis = millis();
    if (blinkerOn && (currentMillis - blinkerLastTime > BLINK_ON_MS)) {
        blinkerLastTime = currentMillis;
        blinkerOn = !blinkerOn;
    } else if (!blinkerOn && (currentMillis - blinkerLastTime > BLINK_OFF_MS)) {
        blinkerLastTime = currentMillis;
        blinkerOn = !blinkerOn;
    }
    return blinkerOn;
}

void CharacterDisplayRenderer::resetBlinkerOn() {
    blinkerOn = true;
    blinkerLastTime = millis();
}

void CharacterDisplayRenderer::moveCursor(uint8_t cursorCol, uint8_t cursorRow) {
    MenuRenderer::moveCursor(cursorCol, cursorRow);
    display->setCursor(cursorCol, cursorRow);
}

uint8_t CharacterDisplayRenderer::getEffectiveCols() const {
    return availableColumns - (cursorIcon != 0 || editCursorIcon != 0 ? 1 : 0);
}