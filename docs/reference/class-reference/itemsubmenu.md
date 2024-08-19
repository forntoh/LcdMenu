# ItemSubMenu

## Constructor

```cpp
ItemSubMenu(const char* text, MenuItem** parent);
```

This constructor creates a new `ItemSubMenu` object with the specified parameters.

**Parameters**

* `text`: The text to display for the sub-menu item.
* `parent`: A pointer to the parent menu item that will contain the sub-menu.

The `ItemSubMenu` class inherits from the `ItemHeader` class and sets the `MENU_ITEM_SUB_MENU` type for the menu item. The `text` parameter specifies the text that will be displayed for the sub-menu item. The `parent` parameter is a pointer to the parent menu item, which will contain the sub-menu item.

Please note that the `parent` parameter is of type `MenuItem**`, indicating a pointer to a pointer to a `MenuItem` object.

## `ITEM_SUBMENU`

```cpp
#define ITEM_SUBMENU(...) (new ItemSubMenu(__VA_ARGS__))
```

The `ITEM_SUBMENU` macro creates a new `ItemSubMenu` object using the provided arguments and returns a pointer to it.

**Usage**

The `ITEM_SUBMENU` macro can be used to conveniently create instances of the `ItemSubMenu` class without explicitly calling the constructor. It simplifies the process of creating sub-menu items by providing a compact syntax.

**Arguments**

The `ITEM_SUBMENU` macro takes the following arguments:

* `...`: Variable number of arguments that are passed to the `ItemSubMenu` constructor. These arguments should match the parameters of the `ItemSubMenu` constructor in the correct order.

By using the `ITEM_SUBMENU` macro, you can create instances of the `ItemSubMenu` class as follows:

```cpp
ITEM_SUBMENU("Sub Menu", &parentMenu)
```

In this example, `"Sub Menu"` is the text to display for the sub menu item, and `&parentMenu` is the pointer to the parent menu item.

Please note that the `ITEM_SUBMENU` macro expands to a dynamically allocated `ItemSubMenu` object, so you need to manage memory accordingly to prevent memory leaks.
