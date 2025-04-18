Item List
---------

The :cpp:class:`ItemList` class is a wrapper around the :cpp:class:`ItemWidget` class that simplifies the creation of menu items with a single :cpp:class:`WidgetList`. It reduces boilerplate code when you want to create an item with only one widget, making your code cleaner and easier to maintain.

This widget is particularly useful when you need to present a list of selectable options to the user, such as colors, days of the week, or numeric values.

Usage
~~~~~

The :cpp:class:`ItemList` class allows you to create a menu item that displays a list of values. It provides two main functions:

1. **`ITEM_LIST`**: Creates an item with a list of values.
2. **`ITEM_LIST_REF`**: Creates an item with a list of values, using a reference to track the currently selected value.

Both functions are wrappers around the :cpp:class:`ItemWidget` class with a :cpp:class:`WidgetList` as the widget.

Example: Simple List
####################

.. code-block:: c++

    std::vector<const char*> colors = {"Red", "Green", "Blue"};

    MENU_SCREEN(mainScreen, mainItems,
        ITEM_LIST("Color", colors, [](const uint8_t color) {
            Serial.println(colors[color]);
        })
    );

In this example, the :cpp:class:`ItemList` allows the user to select a color from the list. The selected color is printed to the serial monitor.

Example: List with Reference
############################

.. code-block:: c++

    uint8_t selectedColor = 0;
    std::vector<const char*> colors = {"Red", "Green", "Blue"};

    MENU_SCREEN(mainScreen, mainItems,
        ITEM_LIST_REF("Color", colors, [](const Ref<uint8_t> color) {
            Serial.println(colors[color.value]);
        }, selectedColor)
    );

In this example, the :cpp:class:`ItemList` uses a reference to track the currently selected color. This is useful when the selected value needs to be updated externally.

Scenarios
~~~~~~~~~

The :cpp:class:`ItemList` is ideal for scenarios where you need to:

- Present a list of options to the user.
- Reduce boilerplate code when creating items with a single :cpp:class:`WidgetList`.
- Dynamically update the selected value using a reference.

Best Practices
~~~~~~~~~~~~~~~

When using :cpp:class:`ItemList`, consider the following best practices:

- Ensure the list of options is not too long to avoid overwhelming the user.
- Use meaningful labels for the options to make it easier for the user to understand their choices.
- For performance-critical applications, use simple types (e.g., ``uint8_t``, ``char *``) for the list items rather than complex types.
- The list values are stored in memory, so ensure that the list is not too large to avoid memory issues.
- When using :cpp:func:`ITEM_LIST_REF`, be aware that external changes to the referenced value will be reflected in the menu display. 

For more details, refer to the :cpp:class:`WidgetList` documentation: :doc:`../widgets/widget-list`.