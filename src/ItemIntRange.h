#ifndef ItemIntRange_H
#define ItemIntRange_H

#include "ItemRangeBase.h"

/**
 * @brief Item that allows user to input integer information within a range.
 * The value can be incremented or decremented by a specified step.
 */
class ItemIntRange : public ItemRangeBase<int> {
  public:
    ItemIntRange(
        const char* text,
        const int min,
        const int max,
        int startingValue,
        fptrInt callback,
        const char* format = "%d",
        int step = 1,
        bool commitOnChange = false)
        : ItemRangeBase(text, min, max, startingValue, callback, format, step, commitOnChange) {}

    char* getDisplayValue() override {
        static char buffer[10];
        sprintf(buffer, format, currentValue);
        return buffer;
    }
};

#define ITEM_INT_RANGE(...) (new ItemIntRange(__VA_ARGS__))

#endif  // ItemIntRange_H