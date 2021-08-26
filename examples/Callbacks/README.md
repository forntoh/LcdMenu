# Example 03 Callbacks

This is a basic example, it will show you how to use callback functions in the LcdMenu library.

Callbacks can be used with menu items of type `ItemCommand`, `ItemInput`, `ItemList` or `ItemToggle`

![Callback](https://i.imgur.com/1CUPScl.gif)

## Requirements

- Keyboard
- LcdDisplay

## Code

### 1 Declare the callback function

```cpp
// ../../examples/Callbacks/Callbacks.ino#L36-L36
```

### 1 Add callback to MenuItem

```cpp
// ../../examples/Callbacks/Callbacks.ino#L51-L56
```

### 1 Define the callback function

When `enter()` is invoked, the command _(callback)_ bound to the item is invoked.

```cpp
// ../../examples/Callbacks/Callbacks.ino#L88-L95
```

Full code 👉 [.../examples/Callbacks/Callbacks.ino](https://github.com/forntoh/LcdMenu/tree/master/examples/Callbacks/Callbacks.ino)
