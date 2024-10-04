#pragma once
#include <Arduino.h>
//
#ifndef CURSOR_ICON
#define CURSOR_ICON 0x7E  // →
#endif
#ifndef EDIT_CURSOR_ICON
#define EDIT_CURSOR_ICON 0x7F  // ←
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