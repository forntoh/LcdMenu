#ifndef MenuUtils_H
#define MenuUtils_H
#include <Arduino.h>

#include "constants.h"

inline void substring(const char* str, uint8_t start, uint8_t size, char* substr) {
    strncpy(substr, str + start, size);
    substr[size] = '\0';
}

inline void concat(const char* first, char second, const char* third, char* result) {
    size_t len1 = strlen(first);
    size_t len3 = strlen(third);

    memcpy(result, first, len1);
    result[len1] = second;
    memcpy(result + len1 + 1, third, len3);
    result[len1 + 1 + len3] = '\0';
}

inline void concat(const char* first, char second, char* result) {
    size_t len1 = strlen(first);
    memcpy(result, first, len1);
    result[len1] = second;
    result[len1 + 1] = '\0';
}

inline void concat(const char* first, const char* second, char* result) {
    strcpy(result, first);
    strcat(result, second);
}

inline void remove(char* str, uint8_t index, uint8_t count) {
    uint8_t len = strlen(str);
    if (index + count > len) {
        count = len - index;
    }
    memmove(str + index, str + index + count, len - count - index + 1);
}

inline long mapProgress(uint16_t progress, long minValue, long maxValue) {
    // Map the progress value to a new range (minValue to maxValue)
    return map(progress, MIN_PROGRESS, MAX_PROGRESS, minValue, maxValue);
}

inline float mapProgress(uint16_t progress, float minValue, float maxValue) {
    // Normalize the progress value and map it to the specified floating-point
    // range
    return static_cast<float>(progress) / MAX_PROGRESS * (maxValue - minValue) +
           minValue;
}

inline void printLog(const __FlashStringHelper* command) {
#ifdef DEBUG
    Serial.print(F("#LOG# "));
    Serial.println(command);
#endif
}

inline void printLog(const __FlashStringHelper* command, const char value) {
#ifdef DEBUG
    Serial.print(F("#LOG# "));
    Serial.print(command);
    Serial.print(F("="));
    Serial.println(value);
#endif
}

inline void printLog(const __FlashStringHelper* command, const uint8_t value) {
#ifdef DEBUG
    Serial.print(F("#LOG# "));
    Serial.print(command);
    Serial.print(F("="));
    Serial.println(value, DEC);
#endif
}

inline void printLog(const __FlashStringHelper* command, const char* value) {
#ifdef DEBUG
    Serial.print(F("#LOG# "));
    Serial.print(command);
    Serial.print(F("="));
    Serial.println(value);
#endif
}

#endif