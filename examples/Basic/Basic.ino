#include <Keypad.h>
#include <LcdMenu.h>

#define LCD_ROWS 2
#define LCD_COLS 16

void toggleBacklight();

char keys[4][4] = {{'1', '2', '3', 'A'},
                   {'4', '5', '6', 'B'},
                   {'7', '8', '9', 'C'},
                   {'*', '0', '#', 'D'}};

byte colPins[4] = {5, 4, 3, 2};
byte rowPins[4] = {9, 8, 7, 6};

extern MenuItem mainMenu[];

MenuItem mainMenu[] = {ItemHeader(),
                       MenuItem("Start service"),
                       MenuItem("Connect to WiFi"),
                       MenuItem("Settings"),
                       MenuItem("Blink SOS"),
                       MenuItem("Blink random"),
                       ItemFooter()};

LcdMenu menu(LCD_ROWS, LCD_COLS);

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, 4, 4);

void setup() { menu.setupLcdWithMenu(0x27, mainMenu); }

void loop() {
    char key = keypad.getKey();
    if (key == NO_KEY) return;

    switch (key) {
        case 'A':
            menu.up();
            break;
        case 'B':
            menu.down();
            break;
        case 'C':
            menu.enter();
            break;
        case 'D':
            menu.back();
            break;
        default:
            break;
    }
}