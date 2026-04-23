#pragma once

#include "MenuItem.h"
#include "renderer/GraphicalMenuItem.h"

inline GraphicalMenuItem* asGraphicalMenuItem(MenuItem* item) {
    if (item == NULL) {
        return NULL;
    }

    const void* capability = item->queryCapability(GraphicalMenuItem::capabilityId());
    return const_cast<GraphicalMenuItem*>(static_cast<const GraphicalMenuItem*>(capability));
}

inline const GraphicalMenuItem* asGraphicalMenuItem(const MenuItem* item) {
    if (item == NULL) {
        return NULL;
    }

    const void* capability = item->queryCapability(GraphicalMenuItem::capabilityId());
    return static_cast<const GraphicalMenuItem*>(capability);
}

inline bool setItemFont(MenuItem* item, const uint8_t* font) {
    GraphicalMenuItem* graphicalItem = asGraphicalMenuItem(item);
    if (graphicalItem == NULL) {
        return false;
    }

    graphicalItem->setGraphicalFont(font);
    return true;
}

template <typename T>
inline T* ITEM_FONT(T* item, const uint8_t* font) {
    if (item != NULL) {
        setItemFont(static_cast<MenuItem*>(item), font);
    }
    return item;
}
