---
description: Display navigable menu items on your LCD display with Arduino
---

# Home

LcdMenu is an Arduino library that enables you to create interactive menus and navigation systems for LCD displays. With LcdMenu, you can easily add menus to your projects and navigate through them using buttons or any input device you want. The library supports a wide range of LCD display modules, including character and alphanumeric displays.

![Basic](https://i.imgur.com/nViET8b.gif)

## Features <a href="#features" id="features"></a>

* Dynamic menus: Create menus with multiple levels and submenus.
* Multiple menu types: Choose from different menu types such as command, toggle, and input.
* Callback functions: Assign functions to menu items to execute specific tasks when triggered.

## Installation <a href="#installation" id="installation"></a>

Follow [this guide](https://www.ardu-badge.com/LcdMenu) to install the library with **Arduino Library Manager** or install it with **PlatformIO** using the steps below:

1. Open the PlatformIO IDE or VSCode with PlatformIO extension installed.
2. Create a new project or open an existing one.
3.  Add the LcdMenu library to your project by adding the following line to your `platformio.ini` file:

    ```ini
    lib_deps =
        forntoh/LcdMenu@^3.0.0
    ```
4. Save the changes to the `platformio.ini` file.
5. Build and upload your project to your device.
