#include "GraphicalDisplayRenderer.h"

#include "MenuItem.h"
#include "renderer/GraphicalMenuItem.h"

#include <string.h>

namespace {
const uint8_t listGlyph[] = {0x08, 0x1C, 0x3E, 0x00, 0x3E, 0x1C, 0x08, 0x00};
const uint8_t textBufferSize = ITEM_DRAW_BUFFER_SIZE;

uint8_t safeLength(const char* text) {
    if (text == NULL) {
        return 0;
    }
    size_t len = strlen(text);
    return len > 255 ? 255 : static_cast<uint8_t>(len);
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

const GraphicalMenuItem* asGraphical(const MenuItem* item) {
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
    uint8_t h = gDisplay->getFontHeight();
    uint8_t w = gDisplay->getFontWidth();

    if (h == 0) {
        h = 8;
    }
    if (w == 0) {
        w = 1;
    }

    if (h > maxRowHeight) {
        maxRowHeight = h;
    }
    if (w > maxFontWidth) {
        maxFontWidth = w;
    }
}

void GraphicalDisplayRenderer::applyItemFont(const MenuItem* item) {
    const uint8_t* font = defaultFont;
    const GraphicalMenuItem* graphicalItem = asGraphical(item);
    if (graphicalItem != NULL && graphicalItem->getGraphicalFont() != NULL) {
        font = graphicalItem->getGraphicalFont();
    }
    if (font != NULL) {
        gDisplay->setFont(font);
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

    uint8_t h = rowHeight();
    uint8_t yTop = cursorRow * h;
    uint8_t fontHeight = gDisplay->getFontHeight();
    uint8_t baseline = yTop + (h > fontHeight ? (h + fontHeight) / 2 - 1 : h - 1);

    uint8_t displayWidth = gDisplay->getDisplayWidth();
    uint8_t rightInset = totalItems > getMaxRows() ? scrollbarWidth + scrollbarGap : 0;
    uint8_t contentRight = displayWidth > rightInset ? displayWidth - rightInset : displayWidth;

    gDisplay->setDrawColor(0);
    gDisplay->drawBox(0, yTop, contentRight, h);

    if (hasFocus && !MenuItem::isEditing()) {
        gDisplay->setDrawColor(1);
        gDisplay->drawBox(0, yTop, contentRight, h);
        gDisplay->setDrawColor(0);
    } else {
        gDisplay->setDrawColor(1);
    }

    uint8_t x = leftPadding;
    const char* focusedIcon = MenuItem::isEditing() ? editCursorIcon : cursorIcon;
    uint8_t iconWidth = measureText(focusedIcon);
    if (hasFocus && focusedIcon != NULL && focusedIcon[0] != '\0') {
        gDisplay->setCursor(x, baseline);
        gDisplay->draw(focusedIcon);
    }
    x += iconWidth + cursorGap;

    const char* label = text == NULL ? "" : text;
    gDisplay->setCursor(x, baseline);
    gDisplay->draw(label);
    x += measureText(label) + 1;

    const GraphicalMenuItem* graphicalItem = asGraphical(activeItem);
    bool tightSelection = graphicalItem != NULL && graphicalItem->useTightGraphicalSelectionBox();

    if (value != NULL && value[0] != '\0') {
        uint8_t valueWidth = valueAreaWidth;
        if (valueWidth == 0) {
            valueWidth = measureText(value);
        }
        uint8_t valueRight = contentRight > rightPadding ? contentRight - rightPadding : contentRight;
        uint8_t valueX = valueRight > valueWidth ? valueRight - valueWidth : x;
        gDisplay->setCursor(valueX, baseline);
        gDisplay->draw(value);

        if (hasFocus && MenuItem::isEditing() && hasValueSelection) {
            uint8_t valueLen = safeLength(value);
            uint8_t selectionStart = valueSelectionStart > valueLen ? valueLen : valueSelectionStart;
            uint16_t rawEnd = static_cast<uint16_t>(valueSelectionStart) + valueSelectionLength;
            uint8_t selectionEnd = rawEnd > valueLen ? valueLen : static_cast<uint8_t>(rawEnd);

            if (selectionEnd <= selectionStart && selectionStart < valueLen) {
                selectionEnd = selectionStart + 1;
            }

            if (selectionEnd > selectionStart) {
                char prefixBuf[textBufferSize];
                char selectedBuf[textBufferSize];
                copyTextRange(value, 0, selectionStart, prefixBuf);
                copyTextRange(value, selectionStart, static_cast<uint8_t>(selectionEnd - selectionStart), selectedBuf);

                uint8_t prefixWidth = measureText(prefixBuf);
                uint8_t selectedWidth = measureText(selectedBuf);
                if (selectedWidth == 0) {
                    selectedWidth = gDisplay->getFontWidth() == 0 ? 1 : gDisplay->getFontWidth();
                }

                uint8_t selectedX = valueX + prefixWidth;
                uint8_t pad = tightSelection ? 0 : 1;
                uint8_t highlightX = selectedX > pad ? static_cast<uint8_t>(selectedX - pad) : 0;
                uint16_t highlightRight = static_cast<uint16_t>(selectedX) + selectedWidth + pad;
                if (highlightRight > valueRight) {
                    highlightRight = valueRight;
                }

                uint8_t highlightWidth = highlightRight > highlightX
                                             ? static_cast<uint8_t>(highlightRight - highlightX)
                                             : 0;
                if (highlightWidth > 0) {
                    gDisplay->setDrawColor(1);
                    gDisplay->drawBox(highlightX, yTop, highlightWidth, h);
                    gDisplay->setDrawColor(0);
                    gDisplay->setCursor(selectedX, baseline);
                    gDisplay->draw(selectedBuf);
                    gDisplay->setDrawColor(1);
                }
            }
        }
    } else {
        if (graphicalItem != NULL && graphicalItem->hasGraphicalToggle()) {
            uint8_t box = toggleIndicatorWidth();
            uint8_t xBox = contentRight > rightPadding + box ? contentRight - rightPadding - box : x;
            uint8_t yBox = yTop + (h > box ? (h - box) / 2 : 0);
            gDisplay->drawFrame(xBox, yBox, box, box);
            if (graphicalItem->graphicalToggleState() && box > 4) {
                gDisplay->drawBox(xBox + 2, yBox + 2, box - 4, box - 4);
            }
        }
    }

    if (cursorRow == 0) {
        drawScrollBar();
    }

    gDisplay->setDrawColor(1);
}

void GraphicalDisplayRenderer::clearBlinker() {}

void GraphicalDisplayRenderer::drawBlinker() {
    if (!MenuItem::isEditing()) {
        return;
    }
    uint8_t h = rowHeight();
    uint8_t top = cursorPixelY + 1 > h ? cursorPixelY + 1 - h : 0;
    gDisplay->drawBox(cursorPixelX, top, 1, h);
}

void GraphicalDisplayRenderer::moveCursor(uint8_t col, uint8_t row) {
    MenuRenderer::moveCursor(col, row);
    uint8_t charW = gDisplay->getFontWidth() == 0 ? 1 : gDisplay->getFontWidth();
    uint8_t h = rowHeight();
    cursorPixelX = col * charW;
    cursorPixelY = row * h + h - 1;
    gDisplay->setCursor(cursorPixelX, cursorPixelY);
}

void GraphicalDisplayRenderer::drawSubMenuIndicator() {
    uint8_t h = rowHeight();
    uint8_t top = cursorRow * h;
    uint8_t rightInset = totalItems > getMaxRows() ? scrollbarWidth + scrollbarGap : 0;
    uint8_t contentRight = gDisplay->getDisplayWidth() > rightInset ? gDisplay->getDisplayWidth() - rightInset : gDisplay->getDisplayWidth();
    uint8_t x = contentRight > rightPadding + submenuGlyphWidth ? contentRight - rightPadding - submenuGlyphWidth : leftPadding;
    uint8_t y = top + (h > submenuGlyphHeight ? (h - submenuGlyphHeight) / 2 : 0);

    gDisplay->setDrawColor(hasFocus && !MenuItem::isEditing() ? 0 : 1);
    gDisplay->drawBox(x, y, 1, 1);
    gDisplay->drawBox(x, y + 1, 2, 1);
    gDisplay->drawBox(x, y + 2, 3, 1);
    gDisplay->drawBox(x, y + 3, 2, 1);
    gDisplay->drawBox(x, y + 4, 1, 1);
    gDisplay->setDrawColor(1);
}

void GraphicalDisplayRenderer::drawListIndicator() {
    uint8_t h = rowHeight();
    uint8_t top = cursorRow * h;
    uint8_t rightInset = totalItems > getMaxRows() ? scrollbarWidth + scrollbarGap : 0;
    uint8_t contentRight = gDisplay->getDisplayWidth() > rightInset ? gDisplay->getDisplayWidth() - rightInset : gDisplay->getDisplayWidth();
    uint8_t x = contentRight > rightPadding + listGlyphWidth ? contentRight - rightPadding - listGlyphWidth : leftPadding;
    uint8_t y = top + (h > listGlyphHeight ? (h - listGlyphHeight) / 2 : 0);

    gDisplay->setDrawColor(hasFocus && !MenuItem::isEditing() ? 0 : 1);
    gDisplay->drawXbm(x, y, listGlyphWidth, listGlyphHeight, listGlyph);
    gDisplay->setDrawColor(1);
}

uint8_t GraphicalDisplayRenderer::measureText(const char* text) const {
    if (text == NULL) {
        return 0;
    }
    return gDisplay->getTextWidth(text);
}

uint8_t GraphicalDisplayRenderer::toggleIndicatorWidth() const {
    uint8_t h = rowHeight();
    return h > 4 ? h - 4 : h;
}

uint8_t GraphicalDisplayRenderer::rowHeight() const {
    return maxRowHeight == 0 ? 8 : maxRowHeight;
}

uint8_t GraphicalDisplayRenderer::getMaxRows() const {
    uint8_t h = rowHeight();
    return h == 0 ? 0 : gDisplay->getDisplayHeight() / h;
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
    uint8_t charW = gDisplay->getFontWidth();
    if (charW == 0) {
        charW = 1;
    }

    uint8_t rightInset = totalItems > getMaxRows() ? scrollbarWidth + scrollbarGap : 0;
    uint8_t usable = gDisplay->getDisplayWidth();
    if (usable <= rightInset + leftPadding) {
        return 0;
    }
    usable -= rightInset + leftPadding;

    uint8_t cols = usable / charW;

    uint8_t iconWidth = measureText(cursorIcon);
    uint8_t editIconWidth = measureText(editCursorIcon);
    if (editIconWidth > iconWidth) {
        iconWidth = editIconWidth;
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
