#ifndef MenuUtils_H
#define MenuUtils_H

#include "constants.h"
#include <Arduino.h>

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

#ifdef DEBUG
#define LOG(...) log(__VA_ARGS__)
inline void log(const __FlashStringHelper* command) {
    Serial.print(F("#LOG# "));
    Serial.println(command);
}

template <typename T>
inline void log(const __FlashStringHelper* command, T value) {
    Serial.print(F("#LOG# "));
    Serial.print(command);
    Serial.print(F("="));
    Serial.println(value);
}
#else
#define LOG(...)  // No-op
#endif

#endif  // MenuUtils_H