#ifndef ItemFloatRange_H
#define ItemFloatRange_H

#include "ItemRangeBase.h"

class ItemFloatRange : public ItemRangeBase<float> {
  private:
    uint8_t decimalPlaces = 1;

  public:
    ItemFloatRange(
        const char* text,
        const float min,
        const float max,
        float startingValue,
        fptrFloat callback,
        const char* unit = NULL,
        float step = 0.1f,
        bool commitOnChange = false,
        uint8_t decimalPlaces = 1)
        : ItemRangeBase(text, min, max, startingValue, callback, unit, step, commitOnChange), decimalPlaces(decimalPlaces) {}

    char* getDisplayValue() override {
        static char buffer[20];
        dtostrf(currentValue, calculateWidth(currentValue, decimalPlaces), decimalPlaces, buffer);
        if (unit == NULL) {
            return buffer;
        }
        concat(buffer, unit, buffer);
        return buffer;
    }

    static uint8_t calculateWidth(float currentValue, uint8_t decimalPlaces) {
        int intPart = static_cast<int>(currentValue);
        int intPartWidth = (intPart == 0) ? 1 : static_cast<int>(log10(abs(intPart))) + 1;
        return intPartWidth + 1 + decimalPlaces;  // +1 for the decimal point
    }
};

#define ITEM_FLOAT_RANGE(...) (new ItemFloatRange(__VA_ARGS__))

#endif  // ItemFloatRange_H