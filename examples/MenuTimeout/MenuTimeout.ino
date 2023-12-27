/*
 Menu Timeout

 https://lcdmenu.forntoh.dev/examples/timeout

*/

#include <LcdMenu.h>
#include <utils/commandProccesors.h>

#define LCD_ROWS 2
#define LCD_COLS 16

// Configure keyboard keys (ASCII)
#define UP 56        // NUMPAD 8
#define DOWN 50      // NUMPAD 2
#define ENTER 53     // NUMPAD 5
#define BACK 55      // NUMPAD 7

MAIN_MENU(
    ITEM_BASIC("Start service"),
    ITEM_BASIC("Connect to WiFi"),
    ITEM_BASIC("Settings"),
    ITEM_BASIC("Blink SOS"),
    ITEM_BASIC("Blink random")
);

LcdMenu menu(LCD_ROWS, LCD_COLS);

void setup() {
    Serial.begin(9600);
    menu.setupLcdWithMenu(0x27, mainMenu, 20000);
}

void loop() {
    /**
     * IMPORTANT: You must call this function for the timeout to work
     * The default timeout is 10000ms
     */
    menu.updateTimer();

    // Listen to key
    if (!Serial.available()) return;
    char command = Serial.read();

    processMenuCommand(menu, command, UP, DOWN, ENTER, BACK);
}