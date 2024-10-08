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
    static_cast<CharacterDisplayInterface*>(display)->createChar(1, upArrow);
    static_cast<CharacterDisplayInterface*>(display)->createChar(2, downArrow);
}

void CharacterDisplayRenderer::drawItem(const char* text, const char* value) {
    char buf[maxCols + 1];

    appendCursorToText(text, buf);

    if (value != NULL) {
        concat(buf, ":", buf);
        concat(buf, value, buf);
    }

    buf[availableColumns] = '\0';
    uint8_t cursorCol = strlen(buf);

    padText(buf, buf);
    appendIndicatorToText(buf, buf);

    display->setCursor(0, cursorRow);
    display->draw(buf);
    moveCursor(cursorCol, cursorRow);
}

void CharacterDisplayRenderer::draw(uint8_t byte) {
    display->draw(byte);
}

void CharacterDisplayRenderer::drawBlinker() {
    static_cast<CharacterDisplayInterface*>(display)->drawBlinker();
}

void CharacterDisplayRenderer::clearBlinker() {
    static_cast<CharacterDisplayInterface*>(display)->clearBlinker();
}

void CharacterDisplayRenderer::moveCursor(uint8_t cursorCol, uint8_t cursorRow) {
    MenuRenderer::moveCursor(cursorCol, cursorRow);
    display->setCursor(cursorCol, cursorRow);
}

void CharacterDisplayRenderer::appendCursorToText(const char* text, char* buf) {
    if (cursorIcon == 0 && editCursorIcon == 0) {
        strncpy(buf, text, maxCols);
        buf[maxCols] = '\0';
        return;
    }

    uint8_t cursor = hasFocus ? (inEditMode ? editCursorIcon : cursorIcon) : ' ';
    buf[0] = cursor;
    strncpy(buf + 1, text, maxCols - 1);
    buf[maxCols] = '\0';
}

void CharacterDisplayRenderer::appendIndicatorToText(const char* text, char* buf) {
    uint8_t indicator = (hasHiddenItemsAbove) ? 1 : ((hasHiddenItemsBelow) ? 2 : 0);
    if (indicator != 0 && upArrow != NULL && downArrow != NULL) {
        concat(text, indicator, buf);
    } else {
        strcpy(buf, text);
        if (upArrow != NULL || downArrow != NULL) {
            strcat(buf, " ");
        }
    }
}

void CharacterDisplayRenderer::padText(const char* text, char* buf) {
    uint8_t textLength = strlen(text);
    uint8_t spaces = (textLength > availableColumns) ? 0 : availableColumns - textLength;
    spaces = constrain(spaces, 0, maxCols);
    strcpy(buf, text);
    memset(buf + textLength, ' ', spaces);
    buf[textLength + spaces] = '\0';
}

uint8_t CharacterDisplayRenderer::getEffectiveCols() const {
    return availableColumns - (cursorIcon != 0 || editCursorIcon != 0 ? 1 : 0);
}