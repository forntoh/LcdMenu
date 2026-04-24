#include "MenuScreen.h"
#include "display/GraphicalDisplayInterface.h"
#include "renderer/FrameLifecycleRenderer.h"
#include "renderer/GraphicalMenuItem.h"
#include "renderer/GraphicalRendererContext.h"

namespace {
uint8_t getVisibleGraphicalValueWidth(
    const std::vector<MenuItem*>& items,
    uint8_t view,
    uint8_t rows,
    GraphicalDisplayInterface* display,
    GraphicalRendererContext* context) {
    if (display == NULL || context == NULL) {
        return 0;
    }

    uint8_t widest = 0;
    for (uint8_t i = 0; i < rows && (view + i) < items.size(); i++) {
        MenuItem* item = items[view + i];
        if (item == NULL) {
            continue;
        }

        context->setActiveItem(item);
        const GraphicalMenuItem* graphicalItem =
            static_cast<const GraphicalMenuItem*>(item->queryCapability(GraphicalMenuItem::capabilityId()));
        uint8_t width = graphicalItem == NULL ? 0 : graphicalItem->measureGraphicalValueWidth(display);
        if (width > widest) {
            widest = width;
        }
    }

    context->setActiveItem(NULL);
    return widest;
}

GraphicalRendererContext* toGraphicalContext(MenuRenderer* renderer) {
    if (renderer == NULL) {
        return NULL;
    }
    return static_cast<GraphicalRendererContext*>(
        renderer->queryExtension(GraphicalRendererContext::extensionId()));
}

FrameLifecycleRenderer* toFrameLifecycle(MenuRenderer* renderer) {
    if (renderer == NULL) {
        return NULL;
    }
    return static_cast<FrameLifecycleRenderer*>(renderer->queryExtension(FrameLifecycleRenderer::extensionId()));
}

GraphicalDisplayInterface* toGraphicalDisplay(MenuRenderer* renderer) {
    GraphicalRendererContext* context = toGraphicalContext(renderer);
    if (context == NULL) {
        return NULL;
    }
    return context->getGraphicalDisplay();
}

void clampViewport(uint8_t& cursor, uint8_t& view, size_t itemCount, uint8_t rows) {
    if (itemCount == 0 || rows == 0) {
        return;
    }

    if (cursor >= itemCount) {
        cursor = itemCount - 1;
    }

    uint8_t maxView = itemCount > rows ? itemCount - rows : 0;
    if (view > maxView) {
        view = maxView;
    }

    if (cursor < view) {
        cursor = view;
    } else if (cursor >= view + rows) {
        cursor = view + rows - 1;
    }
}

uint8_t prepareViewport(
    const std::vector<MenuItem*>& items,
    uint8_t& cursor,
    uint8_t& view,
    MenuRenderer* renderer,
    GraphicalRendererContext* graphicalContext,
    GraphicalDisplayInterface* graphicalDisplay,
    uint8_t rows) {
    clampViewport(cursor, view, items.size(), rows);

    graphicalContext->setViewportContext(view, items.size());

    uint8_t valueWidth = getVisibleGraphicalValueWidth(items, view, rows, graphicalDisplay, graphicalContext);
    uint8_t recalculatedRows = renderer->getMaxRows();
    if (recalculatedRows == 0) {
        recalculatedRows = 1;
    }

    if (recalculatedRows != rows) {
        rows = recalculatedRows;
        clampViewport(cursor, view, items.size(), rows);
        graphicalContext->setViewportContext(view, items.size());
        valueWidth = getVisibleGraphicalValueWidth(items, view, rows, graphicalDisplay, graphicalContext);
    }

    graphicalContext->setValueAreaWidth(valueWidth);
    return rows;
}
}  // namespace

void MenuScreen::setParent(MenuScreen* parent) {
    this->parent = parent;
}

uint8_t MenuScreen::getCursor() {
    return cursor;
}

MenuItem* MenuScreen::getItemAt(uint8_t position) {
    return items[position];
}

MenuItem* MenuScreen::operator[](const uint8_t position) {
    return getItemAt(position);
}

void MenuScreen::setCursor(MenuRenderer* renderer, uint8_t position) {
    if (items.empty()) {
        cursor = 0;
        view = 0;
        draw(renderer);
        return;
    }

    uint8_t constrained = constrain(position, 0, items.size() - 1);
    if (!items[constrained]->isSelectable()) {
        uint8_t forward = constrained;
        while (forward < items.size() && !items[forward]->isSelectable()) {
            forward++;
        }
        if (forward < items.size()) {
            constrained = forward;
        } else {
            int8_t backward = constrained;
            while (backward >= 0 && !items[backward]->isSelectable()) {
                backward--;
            }
            constrained = backward < 0 ? constrained : static_cast<uint8_t>(backward);
        }
    }

    uint8_t previousView = view;
    uint8_t viewSize = renderer->getMaxRows();
    if (viewSize == 0) {
        viewSize = 1;
    }
    if (constrained < view) {
        view = constrained;
    } else if (constrained > (view + (viewSize - 1))) {
        view = constrained - (viewSize - 1);
    }

    if (constrained == cursor && previousView == view) {
        return;
    }

    cursor = constrained;
    draw(renderer);
}

void MenuScreen::draw(MenuRenderer* renderer) {
    GraphicalRendererContext* graphicalContext = toGraphicalContext(renderer);
    FrameLifecycleRenderer* frameLifecycle = toFrameLifecycle(renderer);
    GraphicalDisplayInterface* graphicalDisplay = toGraphicalDisplay(renderer);

    uint8_t rows = renderer->getMaxRows();
    if (rows == 0) {
        return;
    }

    if (items.empty()) {
        cursor = 0;
        view = 0;

        if (graphicalContext != NULL) {
            graphicalContext->setViewportContext(0, 0);
            graphicalContext->setValueAreaWidth(0);
            graphicalContext->setActiveItem(NULL);
        }

        if (frameLifecycle != NULL) {
            frameLifecycle->beginFrame();
            frameLifecycle->endFrame();
        }
        return;
    }

    if (graphicalContext != NULL) {
        rows = prepareViewport(items, cursor, view, renderer, graphicalContext, graphicalDisplay, rows);
    } else {
        clampViewport(cursor, view, items.size(), rows);
    }

    if (frameLifecycle != NULL) {
        frameLifecycle->beginFrame();
    }

    for (uint8_t i = 0; i < rows && (view + i) < items.size(); i++) {
        MenuItem* item = this->items[view + i];
        if (item == NULL) {
            continue;
        }

        syncIndicators(i, renderer);

        if (graphicalContext != NULL) {
            graphicalContext->setActiveItem(item);
        }

        item->draw(renderer);
    }

    if (graphicalContext != NULL) {
        graphicalContext->setActiveItem(NULL);
    }

    if (frameLifecycle != NULL) {
        frameLifecycle->endFrame();
    }
}

void MenuScreen::syncIndicators(uint8_t index, MenuRenderer* renderer) {
    uint8_t rows = renderer->getMaxRows();
    renderer->hasHiddenItemsAbove = index == 0 && view > 0;
    renderer->hasHiddenItemsBelow =
        rows > 0 && index == rows - 1 && (view + rows) < items.size();
    renderer->hasFocus = cursor == view + index;
    renderer->cursorRow = index;
}

bool MenuScreen::process(LcdMenu* menu, const unsigned char command) {
    MenuRenderer* renderer = menu->getRenderer();
    GraphicalRendererContext* graphicalContext = toGraphicalContext(renderer);

    if (graphicalContext != NULL) {
        graphicalContext->setActiveItem(NULL);
    }

    if (!items.empty()) {
        uint8_t focusIndex = cursor >= view ? cursor - view : 0;
        syncIndicators(focusIndex, renderer);
        if (graphicalContext != NULL) {
            graphicalContext->setActiveItem(items[cursor]);
        }

        if (items[cursor]->process(menu, command)) {
            if (graphicalContext != NULL) {
                graphicalContext->setActiveItem(NULL);
            }
            return true;
        }

        if (graphicalContext != NULL) {
            graphicalContext->setActiveItem(NULL);
        }
    }

    switch (command) {
        case UP:
            renderer->viewShift = 0;
            up(renderer);
            return true;
        case DOWN:
            renderer->viewShift = 0;
            down(renderer);
            return true;
        case BACK:
            renderer->viewShift = 0;
            if (parent != NULL) {
                uint8_t parentCursor = parent->getCursor();
                menu->setScreen(parent);
                menu->setCursor(parentCursor);
            }
            LOG(F("MenuScreen::back"));
            return true;
        case RIGHT:
            {
                uint8_t maxCols = renderer->getMaxCols();
                if (maxCols > 0 && renderer->cursorCol >= maxCols - 1) {
                    renderer->viewShift++;
                    draw(renderer);
                }
                LOG(F("MenuScreen::right"), renderer->viewShift);
                return true;
            }
        case LEFT:
            if (renderer->viewShift > 0) {
                renderer->viewShift--;
                draw(renderer);
            }
            LOG(F("MenuScreen::left"), renderer->viewShift);
            return true;
        default:
            return false;
    }
}

void MenuScreen::up(MenuRenderer* renderer) {
    if (items.empty()) {
        cursor = 0;
        view = 0;
        draw(renderer);
        return;
    }

    if (cursor > 0) {
        int16_t target = static_cast<int16_t>(cursor) - 1;
        while (target >= 0 && !items[static_cast<size_t>(target)]->isSelectable()) {
            target--;
        }

        if (target >= 0) {
            setCursor(renderer, static_cast<uint8_t>(target));
        } else if (view > 0) {
            view--;
            draw(renderer);
        }
    } else if (view > 0) {
        view--;
        draw(renderer);
    }
    LOG(F("MenuScreen::up"), cursor);
}

void MenuScreen::down(MenuRenderer* renderer) {
    if (items.empty()) {
        cursor = 0;
        view = 0;
        draw(renderer);
        return;
    }

    if (cursor < items.size() - 1) {
        uint16_t target = static_cast<uint16_t>(cursor) + 1;
        while (target < items.size() && !items[static_cast<size_t>(target)]->isSelectable()) {
            target++;
        }

        if (target < items.size()) {
            setCursor(renderer, static_cast<uint8_t>(target));
        } else if (view + renderer->getMaxRows() < items.size()) {
            view++;
            draw(renderer);
        }
    } else if (view + renderer->getMaxRows() < items.size()) {
        view++;
        draw(renderer);
    }
    LOG(F("MenuScreen::down"), cursor);
}

void MenuScreen::reset(MenuRenderer* renderer) {
    cursor = 0;
    view = 0;
    if (!items.empty() && !items[cursor]->isSelectable()) {
        setCursor(renderer, cursor);
    } else {
        draw(renderer);
    }
}

MenuScreen::MenuScreen(const std::vector<MenuItem*>& items) : items(items) {}

void MenuScreen::addItem(MenuItem* item) {
    items.push_back(item);
}

void MenuScreen::addItemAt(uint8_t position, MenuItem* item) {
    if (position <= items.size()) {
        items.insert(items.begin() + position, item);
    }
}

void MenuScreen::removeItemAt(uint8_t position) {
    if (position < items.size()) {
        items.erase(items.begin() + position);
    }
}

void MenuScreen::removeLastItem() {
    if (items.size() > 0) {
        items.pop_back();
    }
}

void MenuScreen::clear() {
    items.clear();
}

bool MenuScreen::poll(MenuRenderer* renderer, uint16_t pollInterval) {
    GraphicalRendererContext* graphicalContext = toGraphicalContext(renderer);
    GraphicalDisplayInterface* graphicalDisplay = toGraphicalDisplay(renderer);

    static unsigned long lastPollTime = 0;
    if (millis() - lastPollTime < pollInterval) {
        return false;
    }

    lastPollTime = millis();

    if (graphicalContext != NULL) {
        graphicalContext->setActiveItem(NULL);
        graphicalContext->setViewportContext(view, items.size());
    }

    if (items.empty() || MenuItem::isEditing()) {
        return false;
    }

    uint8_t rows = renderer->getMaxRows();
    if (rows == 0) {
        return false;
    }

    if (graphicalContext != NULL) {
        rows = prepareViewport(items, cursor, view, renderer, graphicalContext, graphicalDisplay, rows);
    } else {
        clampViewport(cursor, view, items.size(), rows);
    }

    bool redrawn = false;
    for (uint8_t i = 0; i < rows && (view + i) < items.size(); i++) {
        MenuItem* item = this->items[view + i];
        if (item == NULL || !item->polling) {
            continue;
        }

        syncIndicators(i, renderer);

        if (graphicalContext != NULL) {
            graphicalContext->setActiveItem(item);
        }

        item->draw(renderer);
        redrawn = true;
    }

    if (graphicalContext != NULL) {
        graphicalContext->setActiveItem(NULL);
    }

    return redrawn;
}
