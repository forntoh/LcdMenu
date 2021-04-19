# Example 04 Dynamic Menu

This example will show you how to dynamically create sub menu's during runtime using the LcdMenu library.

![Dynamic](https://i.imgur.com/7eqko4N.gif)

## Requirements

- 4x4 Keypad
- LcdDisplay

## Usage

Building a menu from a list of text

```cpp
// ../../examples/DynamicMenu/DynamicMenu.ino#L53-L54
```

```cpp
// ../../examples/DynamicMenu/DynamicMenu.ino#L86-L105
```

## Function reference

```cpp
/**
 * Call this function to set sub menu items for any main menu item
 *
 * @param position main menu item/where to place the sub menu
 * @param items    sub menu items
 */
void setSubMenu(uint8_t position, MenuItem* items);
```

```cpp
/**
 * Builder function for a sub menu
 * this functions appends a header and a footer to the final item list
 *
 * @param items array of MenuItems for the sub menu
 * @param size size of items array
 * @return MenuItem list (pointer) with header and footer items included
 */
MenuItem* buildSubMenu(MenuItem* items, uint8_t size);
```

## Complete Code

Go to [.../examples/DynamicMenu/DynamicMenu.ino](https://github.com/forntoh/LcdMenu/tree/master/examples/DynamicMenu/DynamicMenu.ino)

## Circuit

<img src="{{ site.baseurl }}/assets/img/circuit.png" alt="Circuit">
