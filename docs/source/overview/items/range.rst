Range item
----------

.. deprecated:: 5.4.0
 
    This menu item type is deprecated and will be removed in versions after 5.4.0.
    Please use :doc:`widget item </overview/items/item-widget>` with WidgetRange instead.
    
    Migration example:
    
    .. code-block:: cpp
    
        // Old approach
        ITEM_INT_RANGE("Brightness", 0, 100, 50, callback)
        ITEM_FLOAT_RANGE("Dist", 0.0f, 100.0f, 5.0f, callback)

        // New approach
        ITEM_WIDGET("Brightness", callback, WIDGET_RANGE(50, 1, 0, 100, "%d", 0, false))
        ITEM_WIDGET("Dist", callback, WIDGET_RANGE(5.0f, 0.1f, 0.0f, 100.0f, "%.1f", 0, false))

The range item is a menu item that allows the user to select a value within a specified range.
It is used to create a menu item that presents a range of values to the user, such as a brightness level or a volume setting.

This can be useful for creating a menu item that allows the user to adjust a setting within a specific range of values.

There are two types of range items: integer range and float range.

Integer range
~~~~~~~~~~~~~

An integer range item can be created using the following syntax:

.. code-block:: cpp

    // ... More menu items
    ITEM_INT_RANGE("Brightness", 0, 100, 50, [](int value) {
        // Callback function to handle value change
        // value is the selected value within the range
        // Do something with the selected value
    })
    // ... More menu items

- The first argument is the name of the range item.
- The second argument is the minimum value of the range.
- The third argument is the maximum value of the range.
- The fourth argument is the initial value of the range.
- The last argument is a callback function that will be called when the value is changed.

When the ``Brightness`` menu item is selected, the user can adjust the brightness level within the range of 0 to 100.

You can also optionally specify formatting for the value by providing a format string argument:

.. code-block:: cpp

    // ... More menu items
    ITEM_INT_RANGE("Volume", 0, 100, 50, [](int value) {
        // Callback function to handle value change
        // value is the selected value within the range
        // Do something with the selected value
    }, "%02ddB"), // Print the value with two digits and the unit string "dB" e.g. "50dB", "05dB"
    // ... More menu items

When the ``Volume`` menu item is selected, the user can adjust the volume level within the range of 0 to 100, and the value will be displayed with the format string ``"%02ddB"``.

.. image:: images/item-int-range.gif
    :width: 400px
    :alt: Example of an integer range menu item

Find more information about the range menu item in the :doc:`API reference </reference/api/ItemIntRange>`.

Float range
~~~~~~~~~~~

A float range item can be created using the following syntax:

.. code-block:: cpp

    // ... More menu items
    ITEM_FLOAT_RANGE("Dist", 0.0f, 100.0f, 5.0f, [](float value) {
        // Callback function to handle value change
        // value is the selected value within the range
        // Do something with the selected value
    }, "%.02f km", 0.5f), // Print the value with two decimal places and the unit string "km" e.g. "5.00 km", "5.50 km"
    // ... More menu items

- The last argument is the step size of the range (the increment or decrement value when changing the value).

When the ``Dist`` menu item is selected, the user can adjust the distance within the range of 0.0 to 100.0

.. image:: images/item-float-range.png
    :width: 400px
    :alt: Example of a float range menu item

.. hint::

    These item types support two ways of committing the value change:
    Committing the value after every change executing the callback function or only when the user exits the item.

    The default behavior is to commit the value only when the user exits the item.
    Check the API reference for more information on how to configure this behavior.

Find more information about the range menu item in the :cpp:class:`API reference <ItemFloatRange>`.