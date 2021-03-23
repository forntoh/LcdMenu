This is a basic example, it will show you how to use callback functions in the LcdMenu library.

Callbacks can be used with menu items of type `ItemCommand` or `ItemToggle`

![Callback](https://i.imgur.com/1CUPScl.gif)

### Requirements
- 4x4 Keypad
- LcdDisplay

### Code

Go to [.../examples/Callbacks/Callbacks.ino](https://github.com/forntoh/LcdMenu/tree/master/examples/Callbacks/Callbacks.ino)

```cpp
#include <Keypad.h>
#include <LcdMenu.h>

#define LCD_ROWS 2
#define LCD_COLS 16

// Declare the call back function
void toggleBacklight();

// Configure keypad keys
char keys[4][4] = {{'1', '2', '3', 'A'},
                   {'4', '5', '6', 'B'},
                   {'7', '8', '9', 'C'},
                   {'*', '0', '#', 'D'}};
                   
// Configure keypad pins
byte colPins[4] = {5, 4, 3, 2};
byte rowPins[4] = {9, 8, 7, 6};

extern MenuItem mainMenu[];
extern MenuItem settingsMenu[];

MenuItem mainMenu[] = {ItemHeader(),
                       MenuItem("Start service"),
                       MenuItem("Connect to WiFi"),
                       ItemSubMenu("Settings", settingsMenu),
                       MenuItem("Blink SOS"),
                       MenuItem("Blink random"),
                       ItemFooter()};
/**
 * Create submenu and precise its parent
 */
MenuItem settingsMenu[] = {ItemSubHeader(mainMenu),
                           //
                           // Include callback in ItemToggle
                           //
                           ItemToggle("Backlight", toggleBacklight),
                           MenuItem("Contrast"),
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
            // callback funtion will be executed when enter is pressed
            menu.enter();
            break;
        case 'D':
            menu.back();
            break;
        default:
            break;
    }
}
/**
 * Define callback
 */
void toggleBacklight() { menu.lcd->setBacklight(menu.getItemAt(menu.getCursorPosition()).isOn); }
```

### Circuit

![Circuit](https://github.com/forntoh/LcdMenu/blob/master/examples/circuit.png)
