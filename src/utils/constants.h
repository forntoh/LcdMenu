#pragma once
#include <Arduino.h>

typedef void (*fptr)();
typedef void (*fptrInt)(uint16_t);
typedef void (*fptrStr)(char*);
typedef char* (*fptrMapping)(uint16_t);
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
// Rotary encoder configuration
//
#ifndef LONG_PRESS_DURATION
#define LONG_PRESS_DURATION 1000
#endif
#ifndef DOUBLE_PRESS_THRESHOLD
#define DOUBLE_PRESS_THRESHOLD 300
#endif
//
const byte DOWN_ARROW[8] = {
    0b00100,  //   *
    0b00100,  //   *
    0b00100,  //   *
    0b00100,  //   *
    0b00100,  //   *
    0b10101,  // * * *
    0b01110,  //  ***
    0b00100   //   *
};
const byte UP_ARROW[8] = {
    0b00100,  //   *
    0b01110,  //  ***
    0b10101,  // * * *
    0b00100,  //   *
    0b00100,  //   *
    0b00100,  //   *
    0b00100,  //   *
    0b00100   //   *
};
//
#define MIN_PROGRESS 0
#define MAX_PROGRESS 1000
//
#ifndef CURSOR_ICON
#define CURSOR_ICON 0x7E  // →
#endif
#ifndef EDIT_CURSOR_ICON
#define EDIT_CURSOR_ICON 0x7F  // ←
#endif
#ifndef DISPLAY_TIMEOUT
#define DISPLAY_TIMEOUT 10000  // 10 seconds
#endif