# Example 04 List

This is a basic example, it will show you how to use lists in the LcdMenu library.

> Requested, inpired and based on previous work by [@thijstriemstra](https://github.com/forntoh/LcdMenu/pull/22) 🙏

The supported datatype for the list is `String`. This can be used for other primitive datatypes, you just need to pass it as string then parse the result to the desired datatype.

## Requirements

- Keyboard
- LcdDisplay

## Code

### 1 Declare the array

```cpp
// ../../examples/List/List.ino#L41-L41
```

### 1 Initialize the array

```cpp
// ../../examples/List/List.ino#L43-L44
```

### 1 Add the array to your `MenuItem`

You must add the size of the array in order for the menu to know when to stop or loop when cycling through the items in the list.

```cpp
// ../../examples/List/List.ino#L58-L58
```

### 1 Cycle through list

Use `menu.left()` and/or `menu.right()` to cycle through the items

- When `left()` is invoked the view cycles down the list
- When `right()` is invoked the view cycles up the list, you can use only `right()` if you have a single button, because once the menu reaches the end of the list, it automatically goes to the begining.

### 1 Run when item is selected

When `enter()` is invoked, the command _(callback)_ bound to the item is invoked.

```cpp
// ../../examples/List/List.ino#L97-L100
```

Full example 👉 [.../examples/List/List.ino](https://github.com/forntoh/LcdMenu/tree/master/examples/List/List.ino)
