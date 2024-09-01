# Migration Guide

## Migrating to LcdMenu v4

This guide details the changes and how to change your code to migrate to LcdMenu v4. [See the full changelog for version v4.0.0](https://github.com/forntoh/LcdMenu/releases/tag/4.0.0)

#### New imports

The biggest change from this version is the separation of the menu drawing logic from the menu control logic.

The first change you'll need to make is to add the import for your desired display interface, [seen all the available interfaces](https://github.com/forntoh/LcdMenu/tree/master/src/interface)

```cpp
#include <interface/LiquidCrystalI2CAdapter.h>
```

#### Construct the display adapter interface

```cpp
LiquidCrystalI2CAdapter lcdAdapter(0x27, LCD_COLS, LCD_ROWS);
```

#### Finally, initialise the  menu

The menu is now initialised differently and is easier than before, replace `menu.setupLcdWithMenu(`<mark style="color:blue;">`0x27`</mark>`, mainMenu)` with

```cpp
menu.initialize(mainMenu);
```

***

#### Other

Cursor icon, edit cursor icon and display timeout are now updated using&#x20;

```cpp
#define CURSOR_ICON 0x00
#define EDIT_CURSOR_ICON 0x00
#define DISPLAY_TIMEOUT 10000
```

{% hint style="info" %}
Note that the lcd instance now resides in the adapter so if you need to perform custom printing or any other function of `LiquidCrystal` like turning on/off backlight, you now need to reference it from the `lcdAdapter` not the menu.
{% endhint %}

