#ifndef MenuUtils_H
#define MenuUtils_H

#include <Arduino.h>

void substring(char* str, uint8_t start, uint8_t size, char* substr);

void concat(const char* first, char second, const char* third, char* result);

void concat(const char* first, char second, char* result);

void concat(const char* first, const char* second, char* result);

void remove(char* str, uint8_t index, uint8_t count);

long mapProgress(uint16_t progress, long minValue, long maxValue);

float mapProgress(uint16_t progress, float minValue, float maxValue);

#endif