Value display item
------------------

The value display item is a menu item that displays a text label and a value bound to a variable.
The value can be of any type, such as a number, string, or boolean, and is displayed next to the label.

The value display item is useful for displaying dynamic data that changes over time.
It can be used to show the current state of a variable or to display a value that is updated by an external source.

A value display item can be created using the following syntax:

.. code-block:: cpp

    float temperature = 25.0;

    ITEM_VALUE("Temperature", temperature, "%.1f C")

ITEM_VALUE takes three parameters: the label text, the variable to bind the value to, and a format string for displaying the value.
The format string is used to format the value before displaying it on the screen.
In this example, the temperature value is displayed with one decimal place followed by the unit "C" for Celsius.

When the value of the temperature variable changes, the value display item will automatically update to reflect the new value
provided that the menu is polled regularly using the :cpp:func:`LcdMenu::poll` function.

Polling the menu
~~~~~~~~~~~~~~~~

The menu system relies on regular polling to update the screen and show the updated values of the variables bound to the menu items.
The :cpp:func:`LcdMenu::poll` function should be called in the main loop of the sketch to update the menu and handle user input.
You can also call the :cpp:func:`LcdMenu::update` function to manually update the menu without waiting for the next poll cycle.

The :cpp:func:`LcdMenu::poll` has an optional parameter that specifies the maximum polling interval in milliseconds. The default interval is 1000 milliseconds.

Example
+++++++

Let's take a look at an example of a value display item that shows the current temperature on an LCD screen:

.. code-block:: cpp

    // ...
    void loop() {
        // Poll the menu to update the screen
        menu.poll();
    }

For a complete example of using the value display item, see the :doc:`value item example </reference/samples/ItemValue>`.

Find more information about the value display item in the :cpp:class:`API reference <ItemValue>`.