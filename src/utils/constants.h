#pragma once
#include <Arduino.h>
//
// Control codes
//
#define BACKSPACE 8  // Backspace
#define ENTER 10     // Enter
#define BACK 27      // Escape
#define UP 128       // >127
#define DOWN 129     // >127
#define RIGHT 130    // >127
#define LEFT 131     // >127
#define CLEAR 132    // >127
//
#ifndef DISPLAY_TIMEOUT
#define DISPLAY_TIMEOUT 10000  // 10 seconds
#endif