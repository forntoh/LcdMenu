---
description: This example will show you how to input using a charset.
---

# Input from Character Set

## Process

### Imports

```cpp
// other includes ...
#include <ItemInput.h>
#include <LcdMenu.h> // Always comes after every item type import
```

### Define your charset (list of accepted/valid chars)

{% hint style="warning" %}
You also need to track the active index of the list&#x20;
{% endhint %}

```cpp
// ../../examples/CharsetInput/CharsetInput.ino#L36-L40

#define CHARSET_SIZE 10
// Create your charset
char charset[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
// Active index of the charset
uint8_t charsetPosition;
```

### Move to an ItemInput menu item

Use `up()` and/or `down()` to navigate to an `ItemInput`.

### Enter edit mode

Execute `enter()` to go to edit mode. _(the cursor will start blinking 😉)_

### Use `up()` and `down()` to cycle through your charset

<mark style="color:purple;">`drawChar(char c)`</mark> is used to display the character without storing the value, the value will be stored only when <mark style="color:red;">**`type(char character)`**</mark> is executed.

<pre class="language-cpp"><code class="lang-cpp">// ...
case UP:
    if (<a data-footnote-ref href="#user-content-fn-1">menu</a>.isInEditMode())  // Update the position only in edit mode
        charsetPosition = (charsetPosition + 1) % CHARSET_SIZE;
    menu.drawChar(charset[charsetPosition]);  // Works only in edit mode
    menu.up();
    break;
case DOWN:
    if (menu.isInEditMode())  // Update the position only in edit mode
        charsetPosition =
            constrain(charsetPosition - 1, 0, CHARSET_SIZE);
    menu.drawChar(charset[charsetPosition]);  // Works only in edit mode
    menu.down();
    break;
// ...
</code></pre>

### Type the selected character

Use <mark style="color:purple;">`menu.type(char character)`</mark> to type the selected character

```cpp
case ENTER:  // Press enter to go to edit mode: for ItemInput
    menu.type(charset[charsetPosition]);  // Works only in edit mode
    menu.enter();
```

### `menu.back()` will exit edit mode

The value of the menu item will be passed through to the callback function attached to this item.

Full example 👉 [.../examples/CharsetInput/CharsetInput.ino](https://github.com/forntoh/LcdMenu/tree/master/examples/CharsetInput/CharsetInput.ino)

[^1]: It is also worth checking if the current it item is an ItemInput item
