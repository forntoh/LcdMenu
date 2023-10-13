---
description: This example will show you how to get started with submenus
---

# Submenu

<div align="left" data-full-width="false">

<img src="https://i.imgur.com/8jALlOH.gif" alt="Menu">

</div>

You have the possibility to attach as many sub-menus as your microcontroller can handle.

### Code example

#### Includes

```cpp
// other includes ...
#include <ItemSubMenu.h>
#include <LcdMenu.h> // Always comes after every item type import
```

#### Declare your menus



```arduino
extern MenuItem* settingsMenu[];
```

You do not need to define the `mainMenu`, it is already defined for you in the <mark style="color:purple;">`MAIN_MENU`</mark> macro.

#### Create the main menu

<pre class="language-arduino"><code class="lang-arduino">// Define the main menu
MAIN_MENU(
    ITEM_BASIC("Start service"),
    ITEM_BASIC("Connect to WiFi"),
    ITEM_SUBMENU("Settings", <a data-footnote-ref href="#user-content-fn-1">settingsMenu</a>),
    ITEM_BASIC("Blink SOS"),
    ITEM_BASIC("Blink random")
);
</code></pre>

#### Create the submenu

<pre class="language-arduino"><code class="lang-arduino">/**
 * Create the submenu and precise its parent
 */
SUB_MENU(<a data-footnote-ref href="#user-content-fn-2">settingsMenu</a>, <a data-footnote-ref href="#user-content-fn-3">mainMenu</a>,
    ITEM_BASIC("Backlight"),
    ITEM_BASIC("Contrast")
);
</code></pre>

Go to [.../examples/SubMenu/SubMenu.ino](https://github.com/forntoh/LcdMenu/tree/master/examples/SubMenu/SubMenu.ino)

[^1]: Make sure you declare it first

[^2]: Submenu

[^3]: Parent menu
