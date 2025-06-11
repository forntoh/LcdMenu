#ifndef ARDUINO_H
#define ARDUINO_H
#include <stdint.h>
#include <stddef.h>
using byte = uint8_t;
using boolean = bool;
inline unsigned long millis() { return 0; }
#endif
