---
description: >-
  This is a basic example, it will show you how to use callback functions in the
  LcdMenu library.
---

# Command & Toggle

Callbacks can be used with menu items of type `ItemCommand`, `ItemInput`, `ItemList`, `ItemProgress` or `ItemToggle`

<div align="left">

<img src="https://i.imgur.com/1CUPScl.gif" alt="Callback">

</div>

### Declare the callback function

```cpp
// For ItemToggle
void myCallbackFunction(uint8_t isOn);
// For ItemCommand
void myCallbackFunction();
```

### Add a callback to MenuItem

```cpp
MAIN_MENU(
    ITEM_BASIC("Start service"),
    ITEM_BASIC("Connect to WiFi"),
    //
    // Include callback in ItemToggle
    //
    ITEM_TOGGLE("Backlight", myCallbackFunction),
    ITEM_BASIC("Blink SOS"),
    ITEM_BASIC("Blink random")
);
```

### Define the callback function

When `enter()` is invoked, the command _(callback)_ bound to the item is invoked.

```cpp
/**
 * Define the callback function
 */
void myCallbackFunction(uint8_t isOn) {
    menu.lcd->setBacklight(isOn);
    // Or this way
    menu.toggleBacklight();
}
```

### Constructor reference

```cpp
ItemCommand(const char* key, fptr callback)
```

```cpp
ItemToggle(const char* key, fptrInt callback)
```

```cpp
ItemToggle(const char* key, const char* textOn, const char* textOff, fptrInt callback)
```

* **key:** display text of the item
* **textOn:** display text when ON
* **textOff:** display text when OFF
* **callback:** reference to a callback function

Full code 👉 [.../examples/Callbacks/Callbacks.ino](https://github.com/forntoh/LcdMenu/tree/master/examples/Callbacks/Callbacks.ino)
