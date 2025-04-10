Item Range
----------

The :cpp:class:`ItemRange` class is a wrapper around the :cpp:class:`ItemWidget` class that simplifies the creation of menu items with a single :cpp:class:`WidgetRange`. It reduces boilerplate code when you want to create an item with only one widget, making your code cleaner and easier to maintain.

This widget is particularly useful when you need to allow the user to select a value within a specific range, such as volume, brightness, or temperature.

Usage
~~~~~

The :cpp:class:`ItemRange` class allows you to create a menu item that displays a range of values. It provides two main functions:

1. **`ITEM_RANGE`**: Creates an item with a range of values.
2. **`ITEM_RANGE_REF`**: Creates an item with a range of values, using a reference to track the currently selected value.

Both functions are wrappers around the :cpp:class:`ItemWidget` class with a :cpp:class:`WidgetRange` as the widget.

### Example: Simple Range

.. code-block:: c++

    MENU_SCREEN(mainScreen, mainItems,
        ITEM_RANGE<int>("Volume", 50, 5, 0, 100, [](const int value) {
            Serial.println(value);
        }, "%d%%")
    );

In this example, the :cpp:class:`ItemRange` allows the user to select a volume level between 0% and 100% in steps of 5%. The selected value is printed to the serial monitor.

Example: Range with Reference
#############################

.. code-block:: c++

    int selectedTemperature = 20;

    MENU_SCREEN(mainScreen, mainItems,
        ITEM_RANGE_REF<int>("Temperature", selectedTemperature, 1, 0, 40, [](const Ref<int> value) {
            Serial.println(value.value);
        }, "%d°C")
    );

In this example, the :cpp:class:`ItemRange` uses a reference to track the currently selected temperature. This is useful when the selected value needs to be updated externally.

Scenarios
~~~~~~~~~

The :cpp:class:`ItemRange` is ideal for scenarios where you need to:

- Allow the user to select a value within a specific range.
- Reduce boilerplate code when creating items with a single :cpp:class:`WidgetRange`.
- Dynamically update the selected value using a reference.

Best Practices
~~~~~~~~~~~~~~~

When using :cpp:class:`ItemRange`, consider the following best practices:

- Ensure the step size divides evenly into the range to avoid unexpected behavior at range boundaries.  
- For performance-critical applications, use simple numeric types (int, float) rather than complex types.  
- When using :cpp:func:`ITEM_RANGE_REF`, be aware that external changes to the referenced value will be reflected in the menu display.  

For more details, refer to the :cpp:class:`WidgetRange` documentation: :doc:`../widgets/widget-range`.