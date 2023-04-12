# Example 05 Simple Input

This is a basic example, it will show you how to use ItemInput in the LcdMenu library.

`ItemInput` has two constructors, one with an initial value of text and the other without. When creating an `ItemInput`, you must provide a callback function which will be executed when the user exits editing mode (the current value of the `ItemInput` is passed through to this function).

## Requirements

- Keyboard
- LcdDisplay

## Code

### 1. Type a character

Use `menu.type(char character)` to type a character

```cpp
else  // Type the character you want
    menu.type(command);
```

### 2. Run when item is selected

When `enter()` is invoked, the command _(callback)_ bound to the item is invoked.

```cpp
// ../../examples/SimpleInput/SimpleInput.ino#L80-L83

void inputCallback(String value) {
    Serial.print(F("# "));
    Serial.println(value);
}
```

Full example 👉 [.../examples/SimpleInput/SimpleInput.ino](https://github.com/forntoh/LcdMenu/tree/master/examples/SimpleInput/SimpleInput.ino)
