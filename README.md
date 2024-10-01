<h1 align="center">
  <picture>
    <source srcset="docs/assets/logo-dark.png" media="(prefers-color-scheme: dark)">
    <img src="docs/assets/logo-light.png" alt="LcdMenu Logo" height="56">
  </picture></img><br>LcdMenu
  </h1>

<p align="center">
  <a href="https://www.ardu-badge.com/LcdMenu">
    <img src="https://www.ardu-badge.com/badge/LcdMenu.svg" alt="Arduino Library Badge"/>
  </a>
  <a href="https://registry.platformio.org/libraries/forntoh/LcdMenu">
    <img src="https://badges.registry.platformio.org/packages/forntoh/library/LcdMenu.svg" alt="PlatformIO Badge"/>
  </a>
  <img src="https://img.shields.io/github/check-runs/forntoh/LcdMenu/master?logo=githubactions&logoColor=%23ffffff" alt="Check Runs Badge"/>
  <a href="https://lcdmen.forntoh.dev">
    <img src="https://img.shields.io/github/actions/workflow/status/forntoh/LcdMenu/docs.yml?label=docs&logo=google%20docs&logoColor=%23efefef" alt="Documentation Status Badge"/>
  </a>
  <a href="https://github.com/forntoh/LcdMenu/commits/master">
    <img src="https://img.shields.io/github/commits-since/forntoh/LcdMenu/latest?color=yellow&logo=semanticrelease" alt="GitHub Commits Badge"/>
  </a>
  <img src="https://visitor-badge.laobi.icu/badge?page_id=forntoh.LcdMenu" alt="Visitors Count"/>
</p>

LcdMenu is an open-source Arduino library for creating menu systems. It is designed to be easy to use and flexible enough to support a wide range of use cases.

With LcdMenu, you can create a menu system for your Arduino project with minimal effort. The library provides a simple API for creating menus and handling user input. There are also a number of built-in [display interfaces](reference/api/display/index) to choose from, including LCD displays and OLED displays _(coming soon)_.

![Example of a menu system created with LcdMenu](https://i.imgur.com/nViET8b.gif)

## Quick Start

LcdMenu is a simple but powerful library for creating menu systems on embedded devices. It is designed to be easy to use and to work with a wide range of display interfaces and provide various helpers for handling user input like rotary encoders and buttons.

### Installation

Follow [this guide](https://www.ardu-badge.com/LcdMenu) to install the library with **Arduino Library Manager** or install it with **PlatformIO** using the steps below:

1. Open the PlatformIO IDE or VSCode with PlatformIO extension installed.

2. Create a new project or open an existing one.

3. Add the LcdMenu library to your project by adding the following line to your `platformio.ini` file:

   ```bash
   lib_deps =
       forntoh/LcdMenu@^5.0.0
   ```

4. Save the changes to the `platformio.ini` file.

5. Build and upload your project to your device.

### Usage

To create a menu system with LcdMenu, you need to define a menu structure, a display interface, and an input method to interact with the menu. For this example, we will use the `LiquidCrystal_I2C` display interface and `KeyboardAdapter` for user input to read from the serial monitor. You can replace these with other display interfaces and input methods as needed or create your own.

```cpp
#include <LcdMenu.h>
#include <MenuScreen.h>
#include <display/LiquidCrystal_I2CAdapter.h>
#include <input/KeyboardAdapter.h>

MENU_SCREEN(mainScreen, mainItems,
    ITEM_BASIC("Item 1"),
    ITEM_BASIC("Item 2"),
    ITEM_BASIC("Item 3"),
    ITEM_BASIC("Item 4"));

LiquidCrystal_I2C lcd(0x27, 16, 2);
LiquidCrystal_I2CAdapter lcdAdapter(&lcd, 16, 2);
LcdMenu menu(lcdAdapter);
KeyboardAdapter keyboard(&menu, &Serial);

void setup() {
    Serial.begin(9600);
    lcdAdapter.begin();
    menu.setScreen(mainScreen);
}

void loop() {
    keyboard.observe();
}
```

This example creates a simple menu with four items and displays it on a 16x2 LCD screen.
The menu is controlled using the serial monitor, where you can navigate through the items using the arrow keys.

For detailed information on how to use LcdMenu, check out the [official docs](https://lcdmen.forntoh.dev)

---

**Have a question/doubt? Check the [Discussions](https://github.com/forntoh/LcdMenu/discussions) tab, maybe your question has already been answered 😉**
