# ItemToggle

## Signature 1

```cpp
ItemToggle(const char* key, fptrInt callback);
```

This constructor creates a new `ItemToggle` object with the specified parameters, using default text values of "ON" and "OFF" for the toggle states.

#### **Parameters**

* `key`: The key of the menu item.
* `callback`: A pointer to the callback function to execute when this menu item is selected.

## Signature 2

```cpp
ItemToggle(const char* key, const char* textOn, const char* textOff, fptrInt callback);
```

This constructor creates a new `ItemToggle` object with the specified parameters, allowing customization of the text values for the toggle states.

#### **Parameters**

* `key`: The key of the menu item.
* `textOn`: The text to display when the toggle is in the "ON" state.
* `textOff`: The text to display when the toggle is in the "OFF" state.
* `callback`: A pointer to the callback function to execute when this menu item is selected.

Please note that both constructors inherit from the `MenuItem` base class and set the `MENU_ITEM_TOGGLE` type for the menu item.

## `ITEM_TOGGLE` macro:

```cpp
#define ITEM_TOGGLE(...) (new ItemToggle(__VA_ARGS__))
```

The `ITEM_TOGGLE` macro creates a new `ItemToggle` object using the provided arguments and returns a pointer to it.

**Usage**

The `ITEM_TOGGLE` macro can be used to conveniently create instances of the `ItemToggle` class without explicitly calling the constructor. It simplifies the process of creating toggle items by providing a compact syntax.

**Arguments**

The `ITEM_TOGGLE` macro takes the following arguments:

* `...`: Variable number of arguments that are passed to the `ItemToggle` constructor. These arguments should match the parameters of the `ItemToggle` constructor in the correct order.

By using the `ITEM_TOGGLE` macro, you can create instances of the `ItemToggle` class as follows:

```cpp
MenuItem* toggleItem = ITEM_TOGGLE("Toggle Item", "ON", "OFF", callback);
```

In this example, `"Toggle Item"` is the text to display for the toggle item, `"ON"` and `"OFF"` are the texts to display when the toggle is in the "ON" and "OFF" states respectively, and `callback` is a pointer to the callback function to be invoked when the toggle state changes.

Please note that the `ITEM_TOGGLE` macro expands to a dynamically allocated `ItemToggle` object, so you need to manage memory accordingly to prevent memory leaks.
