# ItemInput

The `ItemInput` class represents an input item in a menu. It allows the user to enter a value and provides a callback function to handle the submitted value.

## Constructors

### **ItemInput(const char\* text, char\* value, fptrStr callback)**

Constructs a new `ItemInput` object with an initial value.

* `text`: The text to display for the item.
* `value`: A pointer to the initial value for the input.
* `callback`: A reference to the callback function to be invoked when the input is submitted.

Example usage:

```cpp
char inputValue[20]; // Define a buffer to hold the input value

void inputCallback(const char* value) {
    // Handle the submitted input value
    // ...
}

ItemInput inputItem("Enter Value:", inputValue, inputCallback);
```

### **ItemInput(const char\* text, fptrStr callback)**

Constructs a new `ItemInput` object with no initial value.

* `text`: The text to display for the item.
* `callback`: A reference to the callback function to be invoked when the input is submitted.

Example usage:

```cpp
void inputCallback(const char* value) {
    // Handle the submitted input value
    // ...
}

ItemInput inputItem("Enter Value:", inputCallback);
```

In this case, the initial value for the input will be an empty string (`""`).

#### Additional Information

The `ItemInput` class inherits from the `MenuItem` class and provides functionality specific to input items in a menu. It allows users to enter values and provides a callback function to handle the submitted values.

Please note that when using the `ItemInput` class, you need to manage the memory of the `value` buffer appropriately to prevent memory leaks or access violations.

## `ITEM_INPUT` macro

```cpp
#define ITEM_INPUT(...) (new ItemInput(__VA_ARGS__))
```

Creates a new `ItemInput` object using the provided arguments and returns a pointer to it.

**Usage**

The `ITEM_INPUT` macro can be used to conveniently create instances of the `ItemInput` class without explicitly calling the constructor. It simplifies the process of creating input items by providing a compact syntax.

**Arguments**

The `ITEM_INPUT` macro takes the following arguments:

* `...`: Variable number of arguments that are passed to the `ItemInput` constructor. These arguments should match the parameters of the `ItemInput` constructor in the correct order.

By using the `ITEM_INPUT` macro, you can create instances of the `ItemInput` class as follows:

```cpp
ITEM_INPUT("Input Item", value, callback)
```

In this example, `"Input Item"` is the text to display for the input item, `value` is a pointer to the initial value for the input, and `callback` is a reference to the callback function to be invoked when the input is submitted.

Alternatively, if you don't need an initial value, you can create an `ItemInput` object as follows:

```cpp
ITEM_INPUT("Input Item", callback)
```

In this case, the initial value will be an empty string (`""`).

Please note that the `ITEM_INPUT` macro expands to a dynamically allocated `ItemInput` object, so you need to manage memory accordingly to prevent memory leaks.
