#pragma once

#if defined(__has_include)
#if !__has_include(<vector>) || !__has_include(<functional>)
#include <ArduinoSTL.h>
#endif
#else
// Fallback: for compilers that don’t support __has_include,
#if !defined(ARDUINO_ARCH_ESP32) &&   \
    !defined(ARDUINO_ARCH_ESP8266) && \
    !defined(ARDUINO_ARCH_STM32) &&   \
    !defined(ARDUINO_ARCH_SAMD)
#include <ArduinoSTL.h>
#endif
#endif
