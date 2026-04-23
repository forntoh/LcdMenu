#include "GraphicalDisplayRenderer.h"

#include "MenuItem.h"
#include "renderer/GraphicalMenuItem.h"

#include <string.h>

namespace {
static const uint8_t updownGlyph[] = {
    0x08,
    0x1C,
    0x3E,
    0x00,
    0x3E,
    0x1C,
    0x08,
};

static const uint8_t textBufferSize = 64;

uint8_t safeLength(const char* text) {
    if (text == NULL) {
        return 0;
    }
    size_t len = strlen(text);
    return len > 255 ? 255 : static_cast<uint8_t>(len);
}

void copyTextWindow(const char* text, uint8_t maxChars, char* out) {
    if (text == NULL || maxChars == 0) {
        out[0] = '\0';
        return;
    }

    uint8_t i = 0;
    while (text[i] != '\0' && i < maxChars && i < textBufferSize - 1) {
        out[i] = text[i];
        i++;
    }
    out[i] = '\0';
}

void copyTextWindowByWidth(const char* text, uint8_t maxPixelWidth, GraphicalDisplayInterface* display, char* out) {
    if (text == NULL || display == NULL || maxPixelWidth == 0) {
        out[0] = '\0';
        return;
    }

    uint8_t i = 0;
    while (text[i] != '\0' && i < textBufferSize - 1) {
        out[i] = text[i];
        out[i + 1] = '\0';

        if (display->getTextWidth(out) > maxPixelWidth) {
            out[i] = '\0';
            return;
        }

        i++;
    }
    out[i] = '\0';
}

void copyTextRange(const char* text, uint8_t start, uint8_t count, char* out) {
    if (text == NULL || count == 0) {
        out[0] = '\0';
        return;
    }

    uint8_t i = 0;
    while (text[start] != '\0' && i < count && i < textBufferSize - 1) {
        out[i] = text[start];
        i++;
        start++;
    }
    out[i] = '\0';
}

const GraphicalMenuItem* toGraphicalMenuItem(const MenuItem* item) {
    if (item == NULL) {
        return NULL;
    }
    const void* capability = item->queryCapability(GraphicalMenuItem::capabilityId());
    return static_cast<const GraphicalMenuItem*>(capability);
}
}  // namespace

GraphicalDisplayRenderer::GraphicalDisplayRenderer(
    GraphicalDisplayInterface* display,
    const uint8_t* defaultFont,
    const char* cursorIcon,
    const char* editCursorIcon)
    : MenuRenderer(display, 0, 0),
      gDisplay(display),
      defaultFont(defaultFont),
      cursorIcon(cursorIcon),
      editCursorIcon(editCursorIcon) {}

void GraphicalDisplayRenderer::setDefaultFont(const uint8_t* font) {
    defaultFont = font;

    if (defaultFont != NULL) {
        gDisplay->setFont(defaultFont);
    }
    captureCurrentFontMetrics();
    applyItemFont(activeItem);
}

bool GraphicalDisplayRenderer::setItemFont(MenuItem* item, const uint8_t* font) {
    if (!::setItemFont(item, font)) {
        return false;
    }

    if (font != NULL) {
        gDisplay->setFont(font);
        captureCurrentFontMetrics();
    }

    applyItemFont(activeItem);
    return true;
}

void GraphicalDisplayRenderer::captureCurrentFontMetrics() {
    uint8_t currentHeight = gDisplay->getFontHeight();
    if (currentHeight == 0) {
        currentHeight = 8;
    }

    uint8_t currentWidth = gDisplay->getFontWidth();
    if (currentWidth == 0) {
        currentWidth = 1;
    }

    if (currentHeight > maxRowHeight) {
        maxRowHeight = currentHeight;
    }
    if (currentWidth > maxFontWidth) {
        maxFontWidth = currentWidth;
    }
}

void GraphicalDisplayRenderer::applyItemFont(const MenuItem* item) {
    const uint8_t* selectedFont = defaultFont;
    const GraphicalMenuItem* graphicalItem = toGraphicalMenuItem(item);
    if (graphicalItem != NULL && graphicalItem->getGraphicalFont() != NULL) {
        selectedFont = graphicalItem->getGraphicalFont();
    }

    if (selectedFont != NULL) {
        gDisplay->setFont(selectedFont);
    }

    captureCurrentFontMetrics();
}

void GraphicalDisplayRenderer::begin() {
    MenuRenderer::begin();

    maxRowHeight = 8;
    maxFontWidth = 1;

    if (defaultFont != NULL) {
        gDisplay->setFont(defaultFont);
    }
    captureCurrentFontMetrics();
    applyItemFont(activeItem);

    beginFrame();
    endFrame();
}

void GraphicalDisplayRenderer::beginFrame() {
    gDisplay->clearBuffer();
}

void GraphicalDisplayRenderer::endFrame() {
    gDisplay->sendBuffer();
}

void GraphicalDisplayRenderer::setViewportContext(uint8_t viewStart, uint8_t totalItems) {
    this->viewStart = viewStart;
    this->totalItems = totalItems;
}

void GraphicalDisplayRenderer::setValueAreaWidth(uint8_t width) {
    valueAreaWidth = width;
}

void GraphicalDisplayRenderer::setActiveItem(const MenuItem* item) {
    activeItem = item;
    clearValueSelection();
    applyItemFont(item);
}

GraphicalDisplayInterface* GraphicalDisplayRenderer::getGraphicalDisplay() {
    return gDisplay;
}

void GraphicalDisplayRenderer::setValueSelection(uint8_t start, uint8_t length) {
    valueSelectionStart = start;
    valueSelectionLength = length;
    hasValueSelection = length > 0;
}

void GraphicalDisplayRenderer::clearValueSelection() {
    valueSelectionStart = 0;
    valueSelectionLength = 0;
    hasValueSelection = false;
}

void* GraphicalDisplayRenderer::queryExtension(uint8_t extensionId) {
    if (extensionId == FrameLifecycleRenderer::extensionId()) {
        return static_cast<FrameLifecycleRenderer*>(this);
    }
    if (extensionId == GraphicalIndicatorRenderer::extensionId()) {
        return static_cast<GraphicalIndicatorRenderer*>(this);
    }
    if (extensionId == GraphicalValueSelectionRenderer::extensionId()) {
        return static_cast<GraphicalValueSelectionRenderer*>(this);
    }
    if (extensionId == GraphicalRendererContext::extensionId()) {
        return static_cast<GraphicalRendererContext*>(this);
    }
    return MenuRenderer::queryExtension(extensionId);
}

const void* GraphicalDisplayRenderer::queryExtension(uint8_t extensionId) const {
    if (extensionId == FrameLifecycleRenderer::extensionId()) {
        return static_cast<const FrameLifecycleRenderer*>(this);
    }
    if (extensionId == GraphicalIndicatorRenderer::extensionId()) {
        return static_cast<const GraphicalIndicatorRenderer*>(this);
    }
    if (extensionId == GraphicalValueSelectionRenderer::extensionId()) {
        return static_cast<const GraphicalValueSelectionRenderer*>(this);
    }
    if (extensionId == GraphicalRendererContext::extensionId()) {
        return static_cast<const GraphicalRendererContext*>(this);
    }
    return MenuRenderer::queryExtension(extensionId);
}

void GraphicalDisplayRenderer::draw(uint8_t byte) {
    gDisplay->draw(byte);
}

void GraphicalDisplayRenderer::drawItem(const char* text, const char* value, bool padWithBlanks) {
    (void)padWithBlanks;

    uint8_t rowH = rowHeight();
    uint8_t displayWidth = gDisplay->getDisplayWidth();
    uint8_t top = cursorRow * rowH;

    uint8_t fontHeight = gDisplay->getFontHeight();
    if (fontHeight == 0 || fontHeight > rowH) {
        fontHeight = rowH;
    }
    uint8_t baseline = top + (rowH - fontHeight) / 2 + fontHeight - 1;

    bool showScrollBar = totalItems > getMaxRows();
    uint8_t rightInset = showScrollBar ? scrollbarWidth + scrollbarGap : 0;
    uint8_t contentRight = displayWidth > rightInset ? displayWidth - rightInset : displayWidth;

    gDisplay->setDrawColor(0);
    if (showScrollBar) {
        gDisplay->drawBox(0, top, contentRight, rowH);
    } else {
        gDisplay->drawBox(0, top, displayWidth, rowH);
    }

    bool editing = MenuItem::isEditing();
    bool highlightRow = hasFocus && !editing;
    if (highlightRow) {
        gDisplay->setDrawColor(1);
        gDisplay->drawBox(0, top, contentRight, rowH);
        gDisplay->setDrawColor(0);
    } else {
        gDisplay->setDrawColor(1);
    }

    uint8_t charW = gDisplay->getFontWidth() == 0 ? 1 : gDisplay->getFontWidth();
    const char* focusedCursorIcon = editing ? editCursorIcon : cursorIcon;
    uint8_t cursorAreaWidth = measureText(focusedCursorIcon);
    uint8_t textX = leftPadding;

    if (cursorAreaWidth > 0) {
        gDisplay->setCursor(textX, baseline);
        if (hasFocus) {
            gDisplay->draw(focusedCursorIcon);
        } else {
            gDisplay->draw(" ");
        }
        textX += cursorAreaWidth + cursorGap;
    }

    const char* labelText = text == NULL ? "" : text;
    uint8_t labelLen = safeLength(labelText);

    const GraphicalMenuItem* graphicalItem = toGraphicalMenuItem(activeItem);
    bool hasToggle = graphicalItem != NULL && graphicalItem->hasGraphicalToggle();
    bool useToggleBox = hasToggle && (value == NULL || value[0] == '\0');
    bool hasValue = value != NULL || hasToggle;
    bool hasListIndicator = graphicalItem != NULL && graphicalItem->hasGraphicalListIndicator();
    bool tightSelectionBox = graphicalItem != NULL && graphicalItem->useTightGraphicalSelectionBox();
    bool widgetEditingSelection = hasFocus && editing && hasValueSelection && value != NULL && !useToggleBox;

    uint8_t valueRight = contentRight > rightPadding ? contentRight - rightPadding : contentRight;
    uint8_t valueLeft = valueRight;
    uint8_t reservedForIndicator = hasListIndicator ? static_cast<uint8_t>(listGlyphWidth + listGap) : 0;
    if (valueRight > reservedForIndicator) {
        valueRight -= reservedForIndicator;
    }

    uint8_t alignedValueWidth = valueAreaWidth;
    if (useToggleBox) {
        alignedValueWidth = toggleIndicatorWidth();
    } else if (value != NULL && alignedValueWidth == 0) {
        alignedValueWidth = contentRight / 3;
    }

    uint8_t maxValueWidth = valueRight > textX ? valueRight - textX : 0;
    if (alignedValueWidth > maxValueWidth) {
        alignedValueWidth = maxValueWidth;
    }

    if (hasValue && alignedValueWidth > 0 && valueRight > alignedValueWidth) {
        valueLeft = valueRight - alignedValueWidth;
    }

    uint8_t labelRight = !hasValue ? valueRight : (valueLeft > 1 ? valueLeft - 1 : valueLeft);

    const char* labelPtr = labelText;
    if (hasFocus && !widgetEditingSelection) {
        labelPtr = viewShift < labelLen ? labelText + viewShift : "";
    }

    uint8_t labelPixelBudget = labelRight > textX ? labelRight - textX : 0;
    char labelBuf[textBufferSize];
    copyTextWindowByWidth(labelPtr, labelPixelBudget, gDisplay, labelBuf);

    uint8_t drawnLabelWidth = measureText(labelBuf);
    if (labelBuf[0] != '\0') {
        gDisplay->setCursor(textX, baseline);
        gDisplay->draw(labelBuf);
    }

    if (hasFocus && editing && hasValue && !useToggleBox) {
        uint16_t desiredValueLeft = static_cast<uint16_t>(textX) + drawnLabelWidth + 1;
        if (desiredValueLeft < valueLeft) {
            uint8_t extra = static_cast<uint8_t>(valueLeft - desiredValueLeft);
            uint16_t expandedWidth = static_cast<uint16_t>(alignedValueWidth) + extra;
            alignedValueWidth = expandedWidth > maxValueWidth ? maxValueWidth : static_cast<uint8_t>(expandedWidth);
            valueLeft = valueRight > alignedValueWidth ? static_cast<uint8_t>(valueRight - alignedValueWidth) : valueLeft;
        }
    }

    uint8_t drawnValueWidth = 0;
    uint8_t valueX = valueLeft;
    uint8_t valueShift = 0;

    if (hasValue && alignedValueWidth > 0) {
        if (useToggleBox) {
            uint8_t boxSize = toggleIndicatorWidth();
            if (boxSize > alignedValueWidth) {
                boxSize = alignedValueWidth;
            }

            valueX = valueRight > boxSize ? valueRight - boxSize : valueLeft;
            uint8_t boxY = top + (rowH > boxSize ? (rowH - boxSize) / 2 : 0);

            gDisplay->setDrawColor(highlightRow ? 0 : 1);
            gDisplay->drawFrame(valueX, boxY, boxSize, boxSize);
            if (graphicalItem->graphicalToggleState() && boxSize > 4) {
                gDisplay->drawBox(valueX + 2, boxY + 2, boxSize - 4, boxSize - 4);
            }
            drawnValueWidth = boxSize;
        } else {
            const char* valuePtr = value;
            uint8_t valueLen = safeLength(value);
            if (hasFocus) {
                if (widgetEditingSelection) {
                    uint8_t selectionStart = valueSelectionStart > valueLen ? valueLen : valueSelectionStart;
                    uint16_t rawSelectionEnd = static_cast<uint16_t>(valueSelectionStart) + valueSelectionLength;
                    uint8_t selectionEnd = rawSelectionEnd > valueLen ? valueLen : static_cast<uint8_t>(rawSelectionEnd);

                    if (selectionEnd <= selectionStart && selectionStart < valueLen) {
                        selectionEnd = selectionStart + 1;
                    }

                    valueShift = selectionStart;

                    while (valueShift > 0) {
                        char rangeBuf[textBufferSize];
                        uint8_t candidateShift = valueShift - 1;
                        uint8_t rangeLen = static_cast<uint8_t>(selectionEnd - candidateShift);
                        copyTextRange(value, candidateShift, rangeLen, rangeBuf);
                        if (measureText(rangeBuf) > alignedValueWidth) {
                            break;
                        }
                        valueShift = candidateShift;
                    }

                    while (valueShift < selectionStart) {
                        char rangeBuf[textBufferSize];
                        uint8_t rangeLen = static_cast<uint8_t>(selectionEnd - valueShift);
                        copyTextRange(value, valueShift, rangeLen, rangeBuf);
                        if (measureText(rangeBuf) <= alignedValueWidth) {
                            break;
                        }
                        valueShift++;
                    }
                } else if (viewShift > labelLen) {
                    valueShift = viewShift - labelLen - 1;
                }
            }

            valuePtr = valueShift < valueLen ? value + valueShift : "";

            char valueBuf[textBufferSize];
            copyTextWindowByWidth(valuePtr, alignedValueWidth, gDisplay, valueBuf);

            drawnValueWidth = measureText(valueBuf);
            valueX = valueRight > drawnValueWidth ? valueRight - drawnValueWidth : valueLeft;

            gDisplay->setDrawColor(highlightRow ? 0 : 1);
            gDisplay->setCursor(valueX, baseline);
            gDisplay->draw(valueBuf);

            if (hasFocus && editing && hasValueSelection) {
                uint16_t valueLen16 = safeLength(value);
                uint16_t selectionStart = valueSelectionStart;
                uint16_t selectionEnd = static_cast<uint16_t>(valueSelectionStart) + valueSelectionLength;

                if (selectionStart > valueLen16) {
                    selectionStart = valueLen16;
                }
                if (selectionEnd > valueLen16) {
                    selectionEnd = valueLen16;
                }
                if (selectionEnd <= selectionStart && selectionStart < valueLen16) {
                    selectionEnd = selectionStart + 1;
                }

                uint16_t visibleStart = valueShift;
                uint8_t visibleChars = safeLength(valueBuf);
                uint16_t visibleEnd = static_cast<uint16_t>(visibleStart) + visibleChars;

                uint16_t overlapStart = selectionStart > visibleStart ? selectionStart : visibleStart;
                uint16_t overlapEnd = selectionEnd < visibleEnd ? selectionEnd : visibleEnd;

                if (overlapEnd > overlapStart) {
                    uint8_t relativeStart = static_cast<uint8_t>(overlapStart - visibleStart);
                    uint8_t relativeLen = static_cast<uint8_t>(overlapEnd - overlapStart);

                    char prefixBuf[textBufferSize];
                    char segmentBuf[textBufferSize];
                    copyTextRange(valueBuf, 0, relativeStart, prefixBuf);
                    copyTextRange(valueBuf, relativeStart, relativeLen, segmentBuf);

                    uint8_t prefixWidth = measureText(prefixBuf);
                    uint8_t segmentWidth = measureText(segmentBuf);
                    if (segmentWidth == 0) {
                        segmentWidth = charW;
                    }

                    uint8_t segmentX = valueX + prefixWidth;
                    uint8_t selectionPad = tightSelectionBox ? 0 : 1;
                    uint8_t highlightX = segmentX > selectionPad ? static_cast<uint8_t>(segmentX - selectionPad) : 0;
                    uint16_t highlightRight = static_cast<uint16_t>(segmentX) + segmentWidth + selectionPad;
                    if (highlightRight > valueRight) {
                        highlightRight = valueRight;
                    }
                    uint8_t highlightWidth = highlightRight > highlightX
                                                 ? static_cast<uint8_t>(highlightRight - highlightX)
                                                 : 0;

                    gDisplay->setDrawColor(1);
                    if (highlightWidth > 0) {
                        gDisplay->drawBox(highlightX, top, highlightWidth, rowH);
                    }

                    gDisplay->setDrawColor(0);
                    gDisplay->setCursor(segmentX, baseline);
                    gDisplay->draw(segmentBuf);

                    gDisplay->setDrawColor(1);
                }
            }
        }
    }

    gDisplay->setDrawColor(1);

    if (hasFocus) {
        uint8_t cursorX = !hasValue
                              ? static_cast<uint8_t>(textX + drawnLabelWidth)
                              : static_cast<uint8_t>(valueX + drawnValueWidth);
        moveCursor(cursorX / charW, cursorRow);
    }

    if (cursorRow == 0) {
        drawScrollBar();
    }
}

void GraphicalDisplayRenderer::clearBlinker() {}

void GraphicalDisplayRenderer::drawBlinker() {
    if (!MenuItem::isEditing()) {
        return;
    }

    uint8_t h = rowHeight();
    uint8_t top = cursorPixelY + 1 > h ? cursorPixelY + 1 - h : 0;

    gDisplay->setDrawColor(1);
    gDisplay->drawBox(cursorPixelX, top + 1, 1, h > 2 ? h - 2 : 1);
}

void GraphicalDisplayRenderer::moveCursor(uint8_t col, uint8_t row) {
    MenuRenderer::moveCursor(col, row);

    uint8_t charW = gDisplay->getFontWidth() == 0 ? 1 : gDisplay->getFontWidth();
    uint8_t rowH = rowHeight();

    cursorPixelX = col * charW;
    cursorPixelY = row * rowH + rowH - 1;
    gDisplay->setCursor(cursorPixelX, cursorPixelY);
}

void GraphicalDisplayRenderer::drawSubMenuIndicator() {
    uint8_t rowH = rowHeight();
    uint8_t top = cursorRow * rowH;

    bool showScrollBar = totalItems > getMaxRows();
    uint8_t rightInset = showScrollBar ? scrollbarWidth + scrollbarGap : 0;
    uint8_t contentRight =
        gDisplay->getDisplayWidth() > rightInset ? gDisplay->getDisplayWidth() - rightInset : gDisplay->getDisplayWidth();

    uint8_t x = contentRight > rightPadding + submenuGlyphWidth ? contentRight - rightPadding - submenuGlyphWidth : leftPadding;
    uint8_t y = top + (rowH > submenuGlyphHeight ? (rowH - submenuGlyphHeight) / 2 : 0);

    gDisplay->setDrawColor(hasFocus && !MenuItem::isEditing() ? 0 : 1);

    gDisplay->drawBox(x, y, 1, 1);
    gDisplay->drawBox(x, y + 1, 2, 1);
    gDisplay->drawBox(x, y + 2, 3, 1);
    gDisplay->drawBox(x, y + 3, 2, 1);
    gDisplay->drawBox(x, y + 4, 1, 1);

    gDisplay->setDrawColor(1);
}

void GraphicalDisplayRenderer::drawListIndicator() {
    uint8_t rowH = rowHeight();
    uint8_t top = cursorRow * rowH;

    bool showScrollBar = totalItems > getMaxRows();
    uint8_t rightInset = showScrollBar ? scrollbarWidth + scrollbarGap : 0;
    uint8_t contentRight =
        gDisplay->getDisplayWidth() > rightInset ? gDisplay->getDisplayWidth() - rightInset : gDisplay->getDisplayWidth();

    uint8_t x = contentRight > rightPadding + listGlyphWidth ? contentRight - rightPadding - listGlyphWidth : leftPadding;
    uint8_t y = top + (rowH > listGlyphHeight ? (rowH - listGlyphHeight) / 2 : 0);

    gDisplay->setDrawColor(hasFocus && !MenuItem::isEditing() ? 0 : 1);
    gDisplay->drawXbm(x, y, listGlyphWidth, listGlyphHeight, updownGlyph);
    gDisplay->setDrawColor(1);
}

uint8_t GraphicalDisplayRenderer::measureText(const char* text) const {
    if (text == NULL) {
        return 0;
    }
    return gDisplay->getTextWidth(text);
}

uint8_t GraphicalDisplayRenderer::toggleIndicatorWidth() const {
    uint8_t width = rowHeight() > 4 ? rowHeight() - 4 : rowHeight();
    return width < 3 ? 3 : width;
}

uint8_t GraphicalDisplayRenderer::rowHeight() const {
    return maxRowHeight == 0 ? 8 : maxRowHeight;
}

uint8_t GraphicalDisplayRenderer::getMaxRows() const {
    uint8_t h = rowHeight();
    if (h == 0) {
        return 0;
    }
    return gDisplay->getDisplayHeight() / h;
}

uint8_t GraphicalDisplayRenderer::getMaxCols() const {
    uint8_t w = maxFontWidth;
    if (w == 0) {
        w = gDisplay->getFontWidth();
    }
    if (w == 0) {
        w = 1;
    }
    return gDisplay->getDisplayWidth() / w;
}

uint8_t GraphicalDisplayRenderer::getEffectiveCols() const {
    uint8_t charW = gDisplay->getFontWidth() == 0 ? 1 : gDisplay->getFontWidth();

    bool showScrollBar = totalItems > getMaxRows();
    uint8_t rightInset = showScrollBar ? scrollbarWidth + scrollbarGap : 0;

    uint8_t usable = gDisplay->getDisplayWidth();
    if (usable <= rightInset + leftPadding) {
        return 0;
    }
    usable -= rightInset + leftPadding;

    uint8_t cols = usable / charW;
    uint8_t iconWidth = measureText(cursorIcon);
    uint8_t editWidth = measureText(editCursorIcon);
    if (editWidth > iconWidth) {
        iconWidth = editWidth;
    }
    uint8_t iconCols = static_cast<uint8_t>((iconWidth + charW - 1) / charW);
    if (cols > iconCols) {
        cols -= iconCols;
    } else {
        cols = 0;
    }

    return cols;
}

void GraphicalDisplayRenderer::drawScrollBar() {
    uint8_t rows = getMaxRows();
    if (rows == 0 || totalItems <= rows) {
        return;
    }

    uint8_t displayWidth = gDisplay->getDisplayWidth();
    uint16_t areaHeight16 = static_cast<uint16_t>(rows) * rowHeight();
    if (areaHeight16 > gDisplay->getDisplayHeight()) {
        areaHeight16 = gDisplay->getDisplayHeight();
    }
    uint8_t areaHeight = static_cast<uint8_t>(areaHeight16);

    uint8_t x = displayWidth - scrollbarWidth;

    gDisplay->setDrawColor(0);
    gDisplay->drawBox(x, 0, scrollbarWidth, areaHeight);
    gDisplay->setDrawColor(1);

    uint8_t handleHeight = (static_cast<uint16_t>(rows) * areaHeight) / totalItems;
    if (handleHeight < 2) {
        handleHeight = 2;
    }

    uint16_t trackRange = areaHeight > handleHeight ? areaHeight - handleHeight : 0;
    uint16_t scrollRange = totalItems > rows ? totalItems - rows : 1;
    uint8_t y = (static_cast<uint16_t>(viewStart) * trackRange) / scrollRange;

    gDisplay->drawBox(x, y, scrollbarWidth, handleHeight);
}
