# Example 01 Basic Navigation

This is a basic example, it will show you how to get started with the LcdMenu library

![Basic](https://i.imgur.com/nViET8b.gif)

## Requirements

- Keyboard
- LcdDisplay

## Code

Go to [.../examples/Basic/Basic.ino](https://github.com/forntoh/LcdMenu/tree/master/examples/Basic/Basic.ino)

```cpp
// ../../examples/Basic/Basic.ino#L35-L77

// Define the main menu
extern MenuItem mainMenu[];

// Initialize the main menu items
MenuItem mainMenu[] = {ItemHeader(),
                       MenuItem("Start service"),
                       MenuItem("Connect to WiFi"),
                       MenuItem("Settings"),
                       MenuItem("Blink SOS"),
                       MenuItem("Blink random"),
                       ItemFooter()};
// Construct the LcdMenu
LcdMenu menu(LCD_ROWS, LCD_COLS);

void setup() {
    Serial.begin(9600);
    // Initialize LcdMenu with the menu items
    menu.setupLcdWithMenu(0x27, mainMenu);
}

void loop() {
    if (!Serial.available()) return;
    char command = Serial.read();

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
        menu.type(command);
}
```
