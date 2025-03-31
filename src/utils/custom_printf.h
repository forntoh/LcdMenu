#pragma once

#ifndef USE_CUSTOM_PRINTF
// Platforms known to have full sprintf support
#if defined(ARDUINO_ARCH_ESP32) || defined(ARDUINO_ARCH_ESP8266) || \
    defined(ARDUINO_ARCH_STM32) || defined(ARDUINO_ARCH_SAMD)
#define USE_CUSTOM_PRINTF 0
#else
#define USE_CUSTOM_PRINTF 1
#endif
#endif

#if USE_CUSTOM_PRINTF
#include "utils/printf.h"
#endif
