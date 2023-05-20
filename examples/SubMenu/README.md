# Example 02 Sub Menu

This example will show you how to get started with submenus

![Menu](https://i.imgur.com/8jALlOH.gif)

## Requirements

- Keyboard
- LCD Display

## Code

### 1 Create the main menu

```cpp
// Define the main menu
MAIN_MENU(
    ITEM_BASIC("Start service"),
    ITEM_BASIC("Connect to WiFi"),
    ITEM_SUBMENU("Settings", settingsMenu),
    ITEM_BASIC("Blink SOS"),
    ITEM_BASIC("Blink random")
);
```

### 2 Create the sub menu

```cpp
/**
 * Create submenu and precise its parent
 */
SUB_MENU(settingsMenu, mainMenu,
    ITEM_BASIC("Backlight"),
    ITEM_BASIC("Contrast")
);
```

Go to [.../examples/SubMenu/SubMenu.ino](https://github.com/forntoh/LcdMenu/tree/master/examples/SubMenu/SubMenu.ino)
