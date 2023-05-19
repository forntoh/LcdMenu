#pragma once
#include <Arduino.h>

char* substring(char* str, uint8_t start, uint8_t size) {
    char* substr = new char[size + 1];
    strncpy(substr, str + start, size);
    substr[size] = '\0';
    return substr;
}

char* concat(char* first, char second, char* third) {
    int len1 = strlen(first);
    int len3 = strlen(third);

    char* result = new char[len1 + 2 + len3];

    strcpy(result, first);
    result[len1] = second;
    strcpy(result + len1 + 1, third);

    return result;
}

char* concat(char* first, char second) {
    int len1 = strlen(first);

    char* result = new char[len1 + 2];

    strcpy(result, first);
    result[len1] = second;
    result[len1 + 1] = '\0';

    return result;
}

void remove(char* str, uint8_t index, uint8_t count) {
    uint8_t len = strlen(str);
    if (index + count > len) {
        count = len - index;
    }
    memmove(str + index, str + index + count, len - count - index + 1);
}