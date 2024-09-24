#ifndef ItemIntRange_H
#define ItemIntRange_H

#include "ItemRangeBase.h"

class ItemIntRange : public ItemRangeBase<int> {
  public:
    ItemIntRange(
        const char* text,
        const int min,
        const int max,
        int startingValue,
        fptrInt callback,
        const char* unit = NULL,
        int step = 1,
        bool commitOnChange = false)
        : ItemRangeBase(text, min, max, startingValue, callback, unit, step, commitOnChange) {}

    char* getDisplayValue() override {
        static char buffer[10];
        itoa(currentValue, buffer, 10);
        if (unit == NULL) {
            return buffer;
        }
        concat(buffer, unit, buffer);
        return buffer;
    }
};

#define ITEM_INT_RANGE(...) (new ItemIntRange(__VA_ARGS__))

#endif  // ItemIntRange_H