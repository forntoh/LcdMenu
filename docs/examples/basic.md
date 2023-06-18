---
description: >-
  This is a basic example, it will show you how to get started with the LcdMenu
  library
---

# Basic

<div align="left">

<img src="https://i.imgur.com/nViET8b.gif" alt="Basic">

</div>

### Code

Go to [.../examples/Basic/Basic.ino](https://github.com/forntoh/LcdMenu/tree/master/examples/Basic/Basic.ino)

```cpp
// ../../examples/Basic/Basic.ino#L35-L68

// Initialize the main menu items
MAIN_MENU(
    ITEM_BASIC("Start service"),
    ITEM_BASIC("Connect to WiFi"),
    ITEM_BASIC("Settings"),
    ITEM_BASIC("Blink SOS"),
    ITEM_BASIC("Blink random")
);
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
}
```
