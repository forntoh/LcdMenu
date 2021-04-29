/*
 Dynamic Menu

 This sketch demostrates how to generate sub menu's during runtime
 using the LcdMenu library.

 Circuit:
 * Arduino Board
 * IR Receiver OUT pin to digital pin 7
 * LCD SLC pin to arduino SLC pin
 * LCD SDA pin to arduino SDA pin

 created 28 April 2021
 by Forntoh Thomas

 This example is in the public domain.

 https://github.com/forntoh/LcdMenu/tree/master/examples/GenerateMenu/GenerateMenu.ino

*/

#include <GenerateMenu.h>
#include <IRremote.h>
#include <LcdMenu.h>
#include <neotimer.h>

#define LCD_ROWS 2
#define LCD_COLS 16

#define UP 0x42
#define DOWN 0x43
#define LEFT 0x44
#define RIGHT 0x45
#define ENTER 0x46
#define BACK 0x11
#define BACKSPACE 0x64
#define CLEAR 0x78

uint8_t previousCommand = 0x00;

// Read only after 200ms after the previous command
Neotimer mytimer = Neotimer(200);

MenuItem* mainMenu;

LcdMenu menu(LCD_ROWS, LCD_COLS);

void setup() {
    // Define formatted string
    char input[] =
        "0200\n0203Settings\n1203WiFi\n0017Backlight\n1015Contrast\n0015User\n1"
        "213Android\n0025Password\n1024Connect\n";

    // Use the string to generate a menu
    mainMenu = generateMenu(input);

    // Add meenu to LCD
    menu.setupLcdWithMenu(0x27, mainMenu);

    // Start IR Receiver
    IrReceiver.begin(7, ENABLE_LED_FEEDBACK, USE_DEFAULT_FEEDBACK_LED_PIN);
}

void loop() {
    if (IrReceiver.decode()) {
        uint8_t command = IrReceiver.decodedIRData.command;

        if (command != previousCommand) {
            if (command == UP)
                menu.up();
            else if (command == DOWN)
                menu.down();
            else if (command == LEFT)
                menu.left();
            else if (command == RIGHT)
                menu.right();
            else if (command == ENTER)
                menu.enter();
            else if (command == BACK)
                menu.back();
            else if (command == CLEAR)
                menu.clear();
            else if (command == BACKSPACE)
                menu.backspace();
            else
                menu.type((String)command);
        }

        previousCommand = command;

        if (mytimer.repeat()) {
            IrReceiver.resume();
            previousCommand = 0;
        }
    }
}