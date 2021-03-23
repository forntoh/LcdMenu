This example will show you how to dynamically create sub menu's during runtime using the LcdMenu library.

![Dynamic](https://i.imgur.com/7eqko4N.gif)

### Requirements
- 4x4 Keypad
- LcdDisplay

### Usage
Building a menu from a list of text

```cpp
// List
char* names[] = {"TP-LINK_AP_F558", "iH2K-7539", "KTA-CONNECT", "SM-G955U241"};

// Create the items
MenuItem myItems[4];

// Loop through list and add each item to the menu
for (uint8_t i = 0; i < 4; i++) {
    // Create MenuItem to be added
    MenuItem wifiParamsMenu[] = {MenuItem(names[i]),
                                 ItemInput("Pass", "", NULL)};
    // Insert MenuItem into array
    myItems[i] = ItemSubMenu(names[i], menu.buildSubMenu(wifiParamsMenu, 2));
}

/** Set the submenu, (precise the position on the display and the items to use).
 *  Use the buildSubMenu function to generate a submenu following LcdMenu's style
 */
menu.setSubMenu(1, menu.buildSubMenu(myItems, 4));
```

### Function reference

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

### Complete Code

Go to [.../examples/DynamicMenu/DynamicMenu.ino](https://github.com/forntoh/LcdMenu/tree/master/examples/DynamicMenu/DynamicMenu.ino)

### Circuit

![Circuit](https://github.com/forntoh/LcdMenu/blob/master/examples/circuit.png)
