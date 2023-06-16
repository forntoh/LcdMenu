# LcdMenu 📟

[![arduino-library-badge](https://www.ardu-badge.com/badge/LcdMenu.svg?)](https://www.ardu-badge.com/LcdMenu)
[![PlatformIO Registry](https://badges.registry.platformio.org/packages/forntoh/library/LcdMenu.svg)](https://registry.platformio.org/libraries/forntoh/LcdMenu)

LcdMenu is an Arduino library that enables you to create interactive menus and navigation systems for LCD displays. With LcdMenu, you can easily add menus to your projects and navigate through them using buttons or any input device you want. The library supports a wide range of LCD display modules, including character and alphanumeric displays.

![Basic](https://i.imgur.com/nViET8b.gif)

## Features

- Dynamic menus: Create menus with multiple levels and submenus.
- Multiple menu types: Choose from different menu types such as command, toggle, and input.
- Callback functions: Assign functions to menu items to execute specific tasks when triggered.

## Installation

Follow [this guide](https://www.ardu-badge.com/LcdMenu) to install the library with **Arduino Library Manager** or install it with **PlatformIO** using the steps below:

1. Open the PlatformIO IDE or VSCode with PlatformIO extension installed.
1. Create a new project or open an existing one.
1. Add the LcdMenu library to your project by adding the following line to your `platformio.ini` file:

   ```makefile
   lib_deps =
       forntoh/LcdMenu@^3.0.0
   ```

1. Save the changes to the `platformio.ini` file.

1. Build and upload your project to your device.

## Usage

To use the LcdMenu library in your project, follow these steps:

#### 1. Include the LcdMenu library in your sketch

```cpp
#include <LcdMenu.h>
```

You will need to add other includes for the types of menu items you wish to use, the available types are described in the next step.

#### 2. Create the main menu, use the provided macro `MAIN_MENU()` e.g.

```js
MAIN_MENU(
  ITEM_INPUT("Connect", resultCallback),
  ITEM_BASIC("Settings"),
  ITEM_COMMAND("Backlight", toggleBacklight),
  ITEM_TOGGLE("Toggle", "ON", "OFF", toggleStuff)
);
```

Replace the sample menu items with your own menu items. Here are the different types of menu items available:

| Type               | Description                                                                 | Import            |
| ------------------ | --------------------------------------------------------------------------- | ----------------- |
| `ITEM_BASIC`       | a basic menu item with **no functionality**                                 | N/A               |
| `ITEM_COMMAND`     | a menu item that **executes** a function when selected                      | `<ItemCommand.h>` |
| `ITEM_TOGGLE`      | a menu item that **toggles** a value when selected                          | `<ItemToggle.h>`  |
| `ITEM_INPUT`       | a menu item that **prompts** the user to enter a value                      | `<ItemInput.h>`   |
| `ITEM_SUBMENU`     | a menu item that leads to a **sub-menu** when selected                      | `<ItemSubMenu.h>` |
| `ITEM_STRING_LIST` | a menu item that displays a value that is chosen form a **list of strings** | `<ItemList.h>`    |

For each menu item, specify the menu item text, and any necessary parameters. For example, in `ITEM_COMMAND("Backlight", toggleBacklight)`, `"Backlight"` is the menu item text and `toggleBacklight` is the function to be executed when the item is selected.

#### 3. Once you have created your menu, initialize LcdMenu with the menu items in the `setup()`

```cpp
menu.setupLcdWithMenu(0x27, mainMenu); //I2C
// or
menu.setupLcdWithMenu(rs, en, d0, d1, d2, d3, mainMenu); // Standard
```

#### 4. In the `loop()` function, define how you want to navigate the menu

You can use any input method of your choice to perform actions on the menu

The most essential actions are:

- `menu.up()` and `menu.down()` - Go up and down the menu
- `menu.left()` and `menu.right()` - if the menu is in edit mode,
  - for `ITEM_INPUT` it moves along the characters of the value.
  - for `ITEM_STRING_LIST` it cycles through the items.
- `menu.enter()` - if the active item is
  - `ITEM_INPUT` or `ITEM_STRING_LIST` it goes into edit mode, if you call it again while in edit mode, it executes the callback bound to the item and exits edit mode.
  - `ITEM_COMAND` or `ITEM_TOGGLE` it executes the bound callback
  - `ITEM_SUBMENU` it enters the sub-menu.
- `menu.back()` - either exits edit mode or goes to back to a parent menu depending on the active item.

Full examples can be found [here](https://github.com/forntoh/LcdMenu/tree/master/examples) 👈

### And that's it! You should now have a fully functional LCD menu system for your Arduino project

---

**Have a question/doubt? Check the [Discussions](https://github.com/forntoh/LcdMenu/discussions) tab, maybe your question has already been answered 😉**
