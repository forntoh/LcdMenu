#pragma once
#include <Arduino.h>

typedef void (*fptr)();
typedef void (*fptrInt)(uint16_t);
typedef void (*fptrStr)(char*);
typedef char* (*fptrMapping)(uint16_t);
//
// menu item types
//
const byte MENU_ITEM_MAIN_MENU_HEADER = 1;
const byte MENU_ITEM_SUB_MENU_HEADER = 2;
const byte MENU_ITEM_SUB_MENU = 3;
const byte MENU_ITEM_COMMAND = 4;
const byte MENU_ITEM_INPUT = 5;
const byte MENU_ITEM_NONE = 6;
const byte MENU_ITEM_TOGGLE = 7;
const byte MENU_ITEM_END_OF_MENU = 8;
const byte MENU_ITEM_LIST = 9;
const byte MENU_ITEM_PROGRESS = 10;
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
#define CURSOR_ICON 0x7E;       // →
#define EDIT_CURSOR_ICON 0x7F;  // ←
#define DISPLAY_TIMEOUT 10000;  // 10 seconds