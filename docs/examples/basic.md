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

```arduino
// Initialize the main menu items
MAIN_MENU(
    ITEM_BASIC("Start service"),
    ITEM_BASIC("Connect to WiFi"),
    ITEM_BASIC("Settings"),
    ITEM_BASIC("Blink SOS"),
    ITEM_BASIC("Blink random")
);

// Construct the display adapter 
LiquidCrystalI2CAdapter lcdAdapter(0x27, LCD_COLS, LCD_ROWS);

// Construct the LcdMenu
LcdMenu menu(lcdAdapter);

void setup() {
    Serial.begin(9600);
    // Initialize LcdMenu with the menu items
    menu.initialize(mainMenu);
}

void loop() {
    if (!Serial.available()) return;
    char command = Serial.read();
    processMenuCommand(menu, command, UP, DOWN, ENTER, BACK);
}
```
