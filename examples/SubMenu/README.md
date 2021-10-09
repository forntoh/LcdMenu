# Example 02 Sub Menu

This example will show you how to get started with submenus

![Menu](https://i.imgur.com/8jALlOH.gif)

## Requirements

- Keyboard
- LCD Display

## Code

### 1 Create the main menu

```cpp
// ../../examples/SubMenu/SubMenu.ino#L39-L46

// Define the main menu
MenuItem mainMenu[] = {ItemHeader(),
                       MenuItem("Start service"),
                       MenuItem("Connect to WiFi"),
                       ItemSubMenu("Settings", settingsMenu),
                       MenuItem("Blink SOS"),
                       MenuItem("Blink random"),
                       ItemFooter()};
```

### 2 Create the sub menu

```cpp
// ../../examples/SubMenu/SubMenu.ino#L47-L53

/**
 * Create submenu and precise its parent
 */
MenuItem settingsMenu[] = {ItemHeader(mainMenu), MenuItem("Backlight"),
                           MenuItem("Contrast"), ItemFooter()};

LcdMenu menu(LCD_ROWS, LCD_COLS);
```

Go to [.../examples/SubMenu/SubMenu.ino](https://github.com/forntoh/LcdMenu/tree/master/examples/SubMenu/SubMenu.ino)
