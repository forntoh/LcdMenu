#include <Arduino.h>

#ifndef MenuConstants_H
#define MenuConstants_H

typedef void (*fptr)();
typedef void (*fptrInt)(uint8_t);
//
// menu item types
//
#define MENU_ITEM_MAIN_MENU_HEADER 1
#define MENU_ITEM_SUB_MENU_HEADER 2
#define MENU_ITEM_SUB_MENU 3
#define MENU_ITEM_COMMAND 4
#define MENU_ITEM_INPUT 5
#define MENU_ITEM_NONE 6
#define MENU_ITEM_TOGGLE 7
#define MENU_ITEM_END_OF_MENU 8
#define MENU_ITEM_LIST 9

#endif