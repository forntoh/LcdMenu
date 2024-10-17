#ifndef ItemFloatRange_H
#define ItemFloatRange_H

#include "ItemRangeBase.h"

/**
 * @brief Item that allows user to input float information within a range.
 * The value can be incremented or decremented by a specified step.
 */
class ItemFloatRange : public ItemRangeBase<float> {
  public:
    ItemFloatRange(
        const char* text,
        const float min,
        const float max,
        float startingValue,
        fptrFloat callback,
        const char* format = "%.2f",
        float step = 0.1f,
        bool commitOnChange = false)
        : ItemRangeBase(text, min, max, startingValue, callback, format, step, commitOnChange) {}

    char* getDisplayValue() override {
        static char buffer[10];
        snprintf(buffer, sizeof(buffer), format, currentValue);
        return buffer;
    }
};

#define ITEM_FLOAT_RANGE(...) (new ItemFloatRange(__VA_ARGS__))

#endif  // ItemFloatRange_H