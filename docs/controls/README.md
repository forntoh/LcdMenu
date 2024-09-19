---
icon: gamepad-modern
description: The following controls exist for the menu
layout:
  title:
    visible: true
  description:
    visible: true
  tableOfContents:
    visible: true
  outline:
    visible: true
  pagination:
    visible: true
---

# Controls

This library already contains pre-made menu control helpers that you can use and extend, check them out in the next page.

Under the hood, these helpers use the following functions, you can use them directly if you don't want to use the provided helpers.

### <mark style="color:blue;">`menu.up()`</mark> and <mark style="color:blue;">`menu.down()`</mark>&#x20;

Navigate up and down the menu.\
When the menu is in edit mode, this action has no effect.

### <mark style="color:blue;">`menu.left()`</mark> and <mark style="color:blue;">`menu.right()`</mark>

If the menu is in edit mode,

* for <mark style="color:orange;">**`ITEM_INPUT`**</mark> it moves along the characters of the value.
* for <mark style="color:orange;">**`ITEM_STRING_LIST`**</mark> it cycles through the items.
* for <mark style="color:orange;">**`ITEM_PROGRESS`**</mark> it decrements and increments the progress respectively

### <mark style="color:blue;">`menu.enter()`</mark>

If the active item is:

* <mark style="color:orange;">**`ITEM_INPUT`**</mark>, <mark style="color:orange;">**`ITEM_STRING_LIST`**</mark>  or <mark style="color:orange;">**`ITEM_PROGRESS`**</mark> It goes into edit mode.
* <mark style="color:orange;">**`ITEM_COMAND`**</mark> or <mark style="color:orange;">**`ITEM_TOGGLE`**</mark> it executes the bound callback
* <mark style="color:orange;">**`ITEM_SUBMENU`**</mark> it enters the sub-menu.

### <mark style="color:blue;">`menu.back()`</mark>

If the current menu displayed is a sub-menu, it navigates back to the parent menu.

If the current item is <mark style="color:orange;">**`ITEM_INPUT`**</mark>, <mark style="color:orange;">**`ITEM_STRING_LIST`**</mark>  or <mark style="color:orange;">**`ITEM_PROGRESS`**</mark> it exits edit mode and executes the callback bound to the item.

### <mark style="color:blue;">`menu.backspace()`</mark>

If the current item is <mark style="color:orange;">**`ITEM_INPUT`**</mark>, it removes one character from the end of the current value.

### <mark style="color:blue;">`menu.type(`</mark><mark style="color:purple;">`character`</mark><mark style="color:blue;">`)`</mark>

If the current item is <mark style="color:orange;">**`ITEM_INPUT`**</mark>, it appends one character at the current cursor position to the current value of the item.

### <mark style="color:blue;">`menu.drawChar(`</mark><mark style="color:purple;">`character`</mark><mark style="color:blue;">`)`</mark>

It does the same as type but doesn't set the value, it just draws it to the display.
