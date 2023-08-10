# FAQs

<details>

<summary>How to update value in a menu item on runtime?</summary>

You can update the value of an `ItemInput` in the `loop` function by calling `menu.getItemAt(``position``)->setValue("your new value here")`

Examples:

1. `menu.getItemAt(``your position``)->setValue("your new value here")`
2. `menu[``your position``]->setValue("your new value here")`
3. `menu[``your position``][``your sub position``].setValue("your new value here")`

`1.` and `2.` are the same, `3.` will change the value of the item at ` ``your sub position`` ` in the submenu at ` ``your position`` `

You can also use `[menu.getCursorPosition()]` in place of `menu.[``your position``]` if your cursor is on the desired menu

<mark style="color:red;">**Index/position starts from 1**</mark>

</details>

<details>

<summary>How to completely hide the menu?</summary>

There is a `hide()` and a `show()` function, calling hide will clear the display, thereby hiding the menu.

</details>
