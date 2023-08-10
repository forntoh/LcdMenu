# FAQs

<details>

<summary>How to update value in a menu item on runtime?</summary>

You can update the value of an `ItemInput` in the `loop` function by calling <mark style="color:purple;">**`menu.getItemAt(`**</mark><mark style="color:orange;">**` ``position`` `**</mark><mark style="color:purple;">**`)->setValue(`**</mark><mark style="color:orange;">**`"your new value here"`**</mark><mark style="color:purple;">**`)`**</mark>

Examples:

1. `menu.getItemAt(``your position``)->setValue("your new value here")`
2. `menu[``your position``]->setValue("your new value here")`
3. `menu[``your position``][``your sub position``].setValue("your new value here")`

`1.` and `2.` are the same, `3.` will change the value of the item at ` ``your sub position`` ` in the submenu at ` ``your position`` `

You can also use `[menu.getCursorPosition()]` in place of `menu.[``your position``]` if your cursor is on your desired menu item.

<mark style="color:red;">**Index/position starts from 1**</mark>

</details>

<details>

<summary>How to completely hide the menu?</summary>

There is a <mark style="color:purple;">**`hide()`**</mark> and a <mark style="color:purple;">**`show()`**</mark> function, calling hide will clear the display, thereby hiding the menu.

</details>

<details>

<summary>How to get the parent menu item from a submenu?</summary>

Getting the item at <mark style="color:orange;">**`index = 0`**</mark> from the menu will give you the **`ItemHeader`**, from there you can get the parent menu of the current menu.

Calling <mark style="color:purple;">**`getSubMenu()`**</mark> on an **`ItemHeader`** returns the parent menu of the submenu where the **`ItemHeader`** is.

</details>
