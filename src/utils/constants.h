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
#define MIN_PROGRESS 0
#define MAX_PROGRESS 1000