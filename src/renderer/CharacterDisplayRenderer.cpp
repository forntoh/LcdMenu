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
    char* buf = new char[maxCols + viewShift + 1];

    if (value != NULL) {
        concat(text, ":", buf);
        concat(buf, value, buf);
    } else {
        strcpy(buf, text);
    }

    if (hasFocus && viewShift > 0) {
        memmove(buf, buf + viewShift, availableColumns);
    }

    appendCursorToText(buf);

    buf[availableColumns] = '\0';
    uint8_t cursorCol = strlen(buf);

    padText(buf, buf);
    appendIndicatorToText(buf);

    display->setCursor(0, cursorRow);
    display->draw(buf);
    if (hasFocus) moveCursor(cursorCol, cursorRow);
    delete[] buf;
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

void CharacterDisplayRenderer::appendCursorToText(char* text) {
    if (cursorIcon == 0 && editCursorIcon == 0) return;

    uint8_t cursor = hasFocus ? (inEditMode ? editCursorIcon : cursorIcon) : ' ';
    uint8_t len = strlen(text);

    memmove(text + 1, text, len + 1);
    text[0] = cursor;
}

void CharacterDisplayRenderer::appendIndicatorToText(char* text) {
    uint8_t indicator = (hasHiddenItemsAbove) ? 1 : ((hasHiddenItemsBelow) ? 2 : 0);
    uint8_t len = strlen(text);
    bool hasArrows = upArrow != NULL && downArrow != NULL;

    if (indicator != 0 && hasArrows) {
        text[len] = indicator;
    } else if (hasArrows) {
        text[len] = ' ';
    } else {
        return;
    }
    text[len + 1] = '\0';
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