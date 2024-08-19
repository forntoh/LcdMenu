# ItemProgress

## Signature 1

```cpp
ItemProgress(const char* key, uint16_t start, uint8_t stepLength, fptrMapping mapping, fptrInt callback);
```

This constructor creates a new `ItemProgress` object with the specified parameters.

#### **Parameters**

* `key`: The key of the menu item.
* `start`: The starting position of the progress value.
* `stepLength`: The length of each step for incrementing or decrementing the progress value.
* `mapping`: A pointer to the mapping function that maps the progress value to a custom representation.
* `callback`: A pointer to the callback function to execute when this menu item is selected.

## Signature 2

```cpp
ItemProgress(const char* key, uint16_t start, fptrInt callback);
```

This constructor creates a new `ItemProgress` object with the specified parameters, using a default step length of 1.

#### **Parameters**

* `key`: The key of the menu item.
* `start`: The starting position of the progress value.
* `callback`: A pointer to the callback function to execute when this menu item is selected.

## Signature 3

```cpp
ItemProgress(const char* key, fptrInt callback);
```

This constructor creates a new `ItemProgress` object with the specified parameters, using a default starting position of 0 and a default step length of 1.

#### **Parameters**

* `key`: The key of the menu item.
* `callback`: A pointer to the callback function to execute when this menu item is selected.

## Signature 4

```cpp
ItemProgress(const char* key, uint8_t stepLength, fptrMapping mapping, fptrInt callback);
```

This constructor creates a new `ItemProgress` object with the specified parameters, using a default starting position of 0.

#### **Parameters**

* `key`: The key of the menu item.
* `stepLength`: The length of each step for incrementing or decrementing the progress value.
* `mapping`: A pointer to the mapping function that maps the progress value to a custom representation.
* `callback`: A pointer to the callback function to execute when this menu item is selected.

Please note that all constructors inherit from the `MenuItem` base class and set the `MENU_ITEM_PROGRESS` type for the menu item.

## `ITEM_PROGRESS` macro:

```cpp
#define ITEM_PROGRESS(...) (new ItemProgress(__VA_ARGS__))
```

The `ITEM_PROGRESS` macro creates a new `ItemProgress` object using the provided arguments and returns a pointer to it.

**Usage**

The `ITEM_PROGRESS` macro can be used to conveniently create instances of the `ItemProgress` class without explicitly calling the constructor. It simplifies the process of creating progress items by providing a compact syntax.

**Arguments**

The `ITEM_PROGRESS` macro takes the following arguments:

* `...`: Variable number of arguments that are passed to the `ItemProgress` constructor. These arguments should match the parameters of the `ItemProgress` constructor in the correct order.

By using the `ITEM_PROGRESS` macro, you can create instances of the `ItemProgress` class as follows:

```cpp
ITEM_PROGRESS("Progress Item", start, mapping, callback)
```

In this example, `"Progress Item"` is the text to display for the progress item, `start` is the starting position of the progress, `mapping` is a pointer to the mapping function (or `NULL` if not used), and `callback` is a pointer to the callback function to be invoked when the user interacts with the progress item.

Please note that the `ITEM_PROGRESS` macro expands to a dynamically allocated `ItemProgress` object, so you need to manage memory accordingly to prevent memory leaks.
