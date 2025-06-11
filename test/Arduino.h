#ifndef ARDUINO_H
#define ARDUINO_H
#include <stdint.h>
#include <stddef.h>
#include <string.h>
using byte = uint8_t;
using boolean = bool;
inline unsigned long millis() { return 0; }
template <typename T, typename A, typename B>
T constrain(T val, A low, B high) {
    if (val < static_cast<T>(low)) return static_cast<T>(low);
    if (val > static_cast<T>(high)) return static_cast<T>(high);
    return val;
}
struct __FlashStringHelper {};
#define F(x) x
#endif
