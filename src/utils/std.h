#pragma once

#if defined(__has_include)
#if !__has_include(<vector>)
#include <StandardCplusplus.h>
#endif
#else
// Fallback: for compilers that don’t support __has_include,
#if !defined(ARDUINO_ARCH_ESP32) &&   \
    !defined(ARDUINO_ARCH_ESP8266) && \
    !defined(ARDUINO_ARCH_STM32) &&   \
    !defined(ARDUINO_ARCH_SAMD)
#include <StandardCplusplus.h>
#endif
#endif
