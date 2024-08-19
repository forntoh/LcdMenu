---
icon: hammer
---

# Usage

To use the LcdMenu library in your project, follow these steps:

#### **1. Include the LcdMenu library in your sketch**

```arduino
#include <LcdMenu.h>
#include <interface/YourDesiredAdapter.h>
```

Find currently available display interfaces [here](https://github.com/forntoh/LcdMenu/blob/decouple-menu-controller-and-display/src/interface)

Optionally add `#include <utils/commandProccesors.h>`, this is a helper for processing menu commands.

You will need to add other includes for the types of menu items you wish to use, the available types are described in the next step.

{% hint style="warning" %}
### <mark style="color:orange;">#include</mark> <mark style="color:blue;">LcdMenu.h</mark> should always be the <mark style="background-color:orange;">last</mark> include statement after all menu-item type imports
{% endhint %}

#### 2. Construct the display adapter

#### **3. Create the main menu, using the provided macro `MAIN_MENU()` e.g.**

```arduino
MAIN_MENU(
  ITEM_INPUT("Connect", resultCallback),
  ITEM_BASIC("Settings"),
  ITEM_COMMAND("Backlight", toggleBacklight),
  ITEM_TOGGLE("Toggle", "ON", "OFF", toggleStuff)
);
```

Replace the sample menu items with your own menu items. Here are the different types of menu items available:

<table data-full-width="false"><thead><tr><th>Type</th><th>Description</th><th>Import</th></tr></thead><tbody><tr><td><code>ITEM_BASIC</code></td><td>a basic menu item with <strong>no functionality</strong></td><td>N/A</td></tr><tr><td><code>ITEM_COMMAND</code></td><td>a menu item that <strong>executes</strong> a function when selected</td><td><code>&#x3C;ItemCommand.h></code></td></tr><tr><td><code>ITEM_TOGGLE</code></td><td>a menu item that <strong>toggles</strong> a value when selected</td><td><code>&#x3C;ItemToggle.h></code></td></tr><tr><td><code>ITEM_INPUT</code></td><td>a menu item that <strong>prompts</strong> the user to enter a value</td><td><code>&#x3C;ItemInput.h></code></td></tr><tr><td><code>ITEM_SUBMENU</code></td><td>a menu item that leads to a <strong>sub-menu</strong> when selected</td><td><code>&#x3C;ItemSubMenu.h></code></td></tr><tr><td><code>ITEM_STRING_LIST</code></td><td>a menu item that displays a value that is chosen form a <strong>list of strings</strong></td><td><code>&#x3C;ItemList.h></code></td></tr><tr><td><code>ITEM_PROGRESS</code></td><td>a menu item that displays a mappable progress value from <strong>0</strong> to <strong>1000</strong>.</td><td><code>&#x3C;ItemProgress.h></code></td></tr></tbody></table>

For each menu item, specify the menu item text, and any necessary parameters. For example, in `ITEM_COMMAND("Backlight", toggleBacklight)`, `"Backlight"` is the menu item text and `toggleBacklight` is the function to be executed when the item is selected.

#### **4. Once you have created your menu, initialize LcdMenu with the menu items in the `setup()`**

```arduino
menu.initialize(mainMenu);
```

#### **5. In the `loop()` function, define how you want to navigate the menu**

You can use any input method of your choice to perform actions on the menu
