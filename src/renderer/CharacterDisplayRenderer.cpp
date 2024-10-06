#include "CharacterDisplayRenderer.h"

CharacterDisplayRenderer::CharacterDisplayRenderer(
    CharacterDisplayInterface* display,
    const uint8_t maxCols,
    const uint8_t maxRows,
    const uint8_t cursorIcon,
    const uint8_t editCursorIcon,
    uint8_t* upArrow,
    uint8_t* downArrow)
    : MenuRenderer(display, maxCols, maxRows), upArrow(upArrow), downArrow(downArrow), cursorIcon(cursorIcon), editCursorIcon(editCursorIcon) {}

void CharacterDisplayRenderer::begin() {
    MenuRenderer::begin();
    static_cast<CharacterDisplayInterface*>(display)->createChar(1, upArrow);
    static_cast<CharacterDisplayInterface*>(display)->createChar(2, downArrow);
}

void CharacterDisplayRenderer::drawItem(uint8_t screenRow, const char* text) {
    MenuRenderer::drawItem(screenRow, text);
    char buf[maxCols + 1];

    appendCursorToText(screenRow, text, buf);
    buf[calculateAvailableLength()] = '\0';
    uint8_t cursorCol = strlen(buf);

    padText(buf, maxCols, buf);
    appendIndicatorToText(screenRow, buf, buf);

    display->setCursor(0, screenRow);
    display->draw(buf);
    moveCursor(cursorCol, screenRow);
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

void CharacterDisplayRenderer::appendCursorToText(uint8_t screenRow, const char* text, char* buf) {
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

void CharacterDisplayRenderer::appendIndicatorToText(uint8_t screenRow, const char* text, char* buf) {
    uint8_t indicator = 0;
    if (upScroll) {
        indicator = 1;
    } else if (downScroll) {
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

void CharacterDisplayRenderer::padText(const char* text, uint8_t itemIndex, char* buf) {
    uint8_t textLength = strlen(text);
    uint8_t spaces = (textLength > calculateAvailableLength()) ? 0 : calculateAvailableLength() - textLength;
    spaces = constrain(spaces, 0, maxCols);
    strcpy(buf, text);
    memset(buf + textLength, ' ', spaces);
    buf[textLength + spaces] = '\0';
}

uint8_t CharacterDisplayRenderer::calculateAvailableLength() {
    return maxCols - (upArrow != NULL || downArrow != NULL ? 1 : 0);
}

void CharacterDisplayRenderer::markUpScroll() {
    upScroll = true;
}

void CharacterDisplayRenderer::clearUpScroll() {
    upScroll = false;
}

void CharacterDisplayRenderer::markDownScroll() {
    downScroll = true;
}

void CharacterDisplayRenderer::clearDownScroll() {
    downScroll = false;
}