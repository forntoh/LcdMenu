---
description: >-
  This is a basic example, it will show you how to use lists in the LcdMenu
  library.
---

# String List

> Requested, inspired and based on previous work by [@thijstriemstra](https://github.com/forntoh/LcdMenu/pull/22) 🙏

The supported datatype for the list is `String`. This can be used for other primitive datatypes, you just need to pass it as a string and then parse the result to the desired datatype.

### Declare the array

```cpp
extern String colors[];
```

### Initialize the array

```cpp
String colors[] = {"Red",  "Green",  "Blue",   "Orange",
                   "Aqua", "Yellow", "Purple", "Pink"};
```

### Add the array to your `MenuItem`

<pre class="language-cpp"><code class="lang-cpp">ITEM_STRING_LIST("Col", colors, 9, <a data-footnote-ref href="#user-content-fn-1">colorsCallback</a>),
</code></pre>

{% hint style="danger" %}
You **must** add the size of the array in order for the menu to know when to stop or loop while cycling through the items in the list.
{% endhint %}

### Enter edit mode

```cpp
menu.enter()
```

### Cycle through the list

Use **`menu.left()`** and/or **`menu.right()`** to cycle through the items

* When **`left()`** is invoked the view cycles down the list
* When **`right()`** is invoked the view cycles up the list, you can use only `right()` if you have a single button, because once the menu reaches the end of the list, it automatically goes to the beginning.

### 5. Run when an item is selected

When `back()` is invoked, the command _(callback)_ bound to the item is invoked.

```cpp
void colorsCallback(uint8_t pos) {
    // do something with the index
    Serial.println(colors[pos]);
}
```

Full example 👉 [.../examples/List/List.ino](https://github.com/forntoh/LcdMenu/tree/master/examples/List/List.ino)

[^1]: Ensure that it is define before you use it
