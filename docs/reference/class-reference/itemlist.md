# ItemList

## Constructor

```cpp
ItemList(const char* key, String* items, const uint8_t itemCount, fptrInt callback);
```

This constructor creates a new `ItemList` object with the specified parameters.

**Parameters**

* `key`: The key of the menu item.
* `items`: An array of `String` objects representing the items to display in the list.
* `itemCount`: The number of items in the `items` array.
* `callback`: A pointer to the callback function to execute when an item is selected from the list.

The `ItemList` class inherits from the `MenuItem` base class and sets the `MENU_ITEM_LIST` type for the menu item.

Please note that the `items` array is expected to be of type `String*`, and the number of items should be provided in the `itemCount` parameter. The `callback` function will be invoked when the user selects an item from the list.

## `ITEM_STRING_LIST` macro:

```cpp
#define ITEM_STRING_LIST(...) (new ItemList(__VA_ARGS__))
```

The `ITEM_STRING_LIST` macro creates a new `ItemList` object using the provided arguments and returns a pointer to it.

**Usage**

The `ITEM_STRING_LIST` macro can be used to conveniently create instances of the `ItemList` class without explicitly calling the constructor. It simplifies the process of creating string list items by providing a compact syntax.

**Arguments**

The `ITEM_STRING_LIST` macro takes the following arguments:

* `...`: Variable number of arguments that are passed to the `ItemList` constructor. These arguments should match the parameters of the `ItemList` constructor in the correct order.

By using the `ITEM_STRING_LIST` macro, you can create instances of the `ItemList` class as follows:

```cpp
ITEM_STRING_LIST("String List", items, itemCount, callback)
```

In this example, `"String List"` is the text to display for the string list item, `items` is the array of string items, `itemCount` is the number of items in the array, and `callback` is a pointer to the callback function to be invoked when the user selects an item from the list.

Please note that the `ITEM_STRING_LIST` macro expands to a dynamically allocated `ItemList` object, so you need to manage memory accordingly to prevent memory leaks.
