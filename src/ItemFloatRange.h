#ifndef ItemFloatRange_H
#define ItemFloatRange_H

#include "ItemRangeBase.h"

/**
 * @brief Item that allows user to input float information within a range.
 * The value can be incremented or decremented by a specified step.
 */
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
        bool commitOnChange = false)
        : ItemRangeBase(text, min, max, startingValue, callback, unit, step, commitOnChange) {
        decimalPlaces = calculateDecimalPlaces(step);
    }

    char* getDisplayValue() override {
        static char buffer[10];
        dtostrf(currentValue, calculateWidth(currentValue, decimalPlaces), decimalPlaces, buffer);
        if (unit == NULL) {
            return buffer;
        }
        snprintf(buffer, 10, "%s%s", buffer, unit);
        return buffer;
    }

    /**
     * @brief Calculates the width of a floating-point number when displayed with a specified number of decimal places.
     *
     * This function computes the width of the string representation of a floating-point number, including the decimal point
     * and the specified number of decimal places.
     *
     * @param currentValue The floating-point number whose width is to be calculated.
     * @param decimalPlaces The number of decimal places to include in the width calculation.
     * @return The width of the floating-point number when displayed as a string.
     */
    static inline uint8_t calculateWidth(float currentValue, uint8_t decimalPlaces) {
        int intPart = static_cast<int>(currentValue);
        int intPartWidth = (intPart == 0) ? 1 : static_cast<int>(log10(abs(intPart))) + 1;
        return intPartWidth + 1 + decimalPlaces;  // +1 for the decimal point
    }

    /**
     * @brief Calculates the number of decimal places in a given floating-point step value.
     *
     * This function converts the floating-point step value to a string representation,
     * then determines the number of decimal places by counting the digits after the
     * decimal point. Trailing zeros are not counted as decimal places.
     *
     * @param step The floating-point step value to analyze.
     * @return The number of decimal places in the step value.
     */
    static uint8_t calculateDecimalPlaces(float step) {
        char buffer[10];
        dtostrf(step, 5, 5, buffer);

        char* decimalPos = strchr(buffer, '.');
        if (decimalPos == NULL) return 0;

        uint8_t numDecimalPlaces = strlen(decimalPos + 1);
        while (numDecimalPlaces > 0 && buffer[strlen(buffer) - 1] == '0') {
            buffer[strlen(buffer) - 1] = '\0';
            numDecimalPlaces--;
        }
        return numDecimalPlaces;
    }
};

#define ITEM_FLOAT_RANGE(...) (new ItemFloatRange(__VA_ARGS__))

#endif  // ItemFloatRange_H