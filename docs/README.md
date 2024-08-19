---
description: Display navigable menu items on your LCD display with Arduino
icon: house
---

# Home

**LcdMenu** is an Arduino library that enables you to create interactive menus and navigation systems for LCD displays. With LcdMenu, you can easily add menus to your projects and navigate through them using buttons or any input device you want. The library supports a wide range of LCD display modules, including character and alphanumeric displays.

<div align="left">

<img src="https://i.imgur.com/nViET8b.gif" alt="Basic">

</div>

## Features <a href="#features" id="features"></a>

* **Dynamic menus**: Create menus with multiple levels and submenus.
* **Multiple menu item types**: Choose from different menu item types such as
  * Command: Execute a function when selected
  * Toggle: Toggle a value when selected
  * Input: Prompt the user to enter a value when selected
  * Sub-menus: Create sub-menus that lead to a separate menu
  * Lists: Create menu items that display a value chosen from a list of strings
  * Editing: Edit menu items and their values in place
  * Navigating: Navigate menus using left, right, up, and down buttons or any input device you want
* **Callback functions**: Assign functions to menu items to execute specific tasks when triggered
* **LCD display modules**: Supports a wide range of LCD display modules, including character and alphanumeric display

{% hint style="info" %}
### 🚀 New in LcdMenu 4.0.0 🚀

#### Display Interface Abstraction

* Introduced a new `DisplayInterface` class, which abstracts the display management and makes it easier to integrate different display types without modifying the core logic.
* This interface allows developers to implement custom display adapters, enabling support for a variety of display modules.



#### Improved Modularity and Flexibility

By decoupling the display management logic from the specific hardware, the system is now more modular and easier to extend with new display types.

[Find all the available adapters here](https://github.com/forntoh/LcdMenu/tree/master/src/interface)

[migration-guide.md](reference/migration-guide.md "mention")
{% endhint %}

## Installation <a href="#installation" id="installation"></a>

Follow [this guide](https://www.ardu-badge.com/LcdMenu) to install the library with **Arduino Library Manager** or install it with **PlatformIO** using the steps below:

1. Open the PlatformIO IDE or VSCode with PlatformIO extension installed.
2. Create a new project or open an existing one.
3.  Add the LcdMenu library to your project by adding the following line to your `platformio.ini` file:

    ```ini
    lib_deps =
        forntoh/LcdMenu@^4.0.0
    ```
4. Save the changes to the `platformio.ini` file.
5. Build and upload your project to your device.
