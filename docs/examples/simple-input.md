---
description: This is a basic example, it will show you how to use ItemInput type.
---

# Simple Input

`ItemInput` has two constructors, one with an initial value of text and the other without. When creating an `ItemInput`, you must provide a callback function which will be executed when the user exits editing mode (the current value of the `ItemInput` is passed through to this function).

### Constructors

```cpp
ItemInput(const char* text, char* value, fptrStr callback)
```

```cpp
ItemInput(const char* text, fptrStr callback)
```

* **text**: The text to display for the item.
* **value**: The initial value for the input.
* **callback**: A reference to the callback function to be invoked when exiting edit mode

### Create ItemInput item

<pre class="language-cpp"><code class="lang-cpp">MAIN_MENU(
    // ...
    ITEM_INPUT("Test", <a data-footnote-ref href="#user-content-fn-1">inputCallback</a>), 
    /// ...
);
</code></pre>

### Type a character

Use `menu.type(char character)` to type a character

```cpp
else  // Type the character you want
    menu.type(command);
```

### Run when the menu leaves edit mode example

```cpp
// ../../examples/SimpleInput/SimpleInput.ino#L80-L83

void inputCallback(String value) {
    Serial.print(F("# "));
    Serial.println(value);
}
```

Full example 👉 [.../examples/SimpleInput/SimpleInput.ino](https://github.com/forntoh/LcdMenu/tree/master/examples/SimpleInput/SimpleInput.ino)

[^1]: Reference to the callback function, must be defined
