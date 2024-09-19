/*
 Menu Item List

 https://lcdmenu.forntoh.dev/examples/string-list

*/

#include <ItemList.h>
#include <LcdMenu.h>
#include <MenuScreen.h>
#include <display/LiquidCrystal_I2CAdapter.h>
#include <utils/SimpleNavConfig.h>

#define LCD_ROWS 2
#define LCD_COLS 16

// Declare the callbacks
void colorsCallback(uint16_t pos);
void numsCallback(uint16_t pos);

// Declare the array
extern String colors[];
// Initialize the array
String colors[] = {"Red", "Green", "Blue", "Orange", "Aqua", "Yellow", "Purple", "Pink"};

// Declare the array
extern String nums[];
// Initialize the array
String nums[] = {"5", "7", "9", "12", "32"};

// Initialize the main menu items
// clang-format off
MENU_SCREEN(mainScreen, mainItems,
    ITEM_BASIC("List demo"),
    ITEM_STRING_LIST("Col", colors, 8, colorsCallback),
    ITEM_STRING_LIST("Num", nums, 5, numsCallback),
    ITEM_BASIC("Example"));
// clang-format on

// Construct the LcdMenu
LiquidCrystal_I2C lcd(0x27, LCD_COLS, LCD_ROWS);
LiquidCrystal_I2CAdapter lcdAdapter(&lcd, LCD_COLS, LCD_ROWS);
LcdMenu menu(lcdAdapter);

SimpleNavConfig navConfig = {
    .menu = &menu,
    .up = 'w',
    .down = 's',
    .enter = ' ',
    .back = 'b',
    .left = 'a',
    .right = 'd',
};

void setup() {
    Serial.begin(9600);
    // Initialize LcdMenu with the menu items
    menu.initialize(mainScreen);
}

void loop() {
    if (!Serial.available()) return;
    char command = Serial.read();
    processWithSimpleCommand(&navConfig, command);
}

// Define the callbacks
void colorsCallback(uint16_t pos) {
    // do something with the index
    Serial.println(colors[pos]);
}

void numsCallback(uint16_t pos) {
    // do something with the index
    Serial.println(nums[pos]);
}
