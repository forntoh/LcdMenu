WidgetRange
===========

The WidgetRange widget is a widget that allows the user to select a value within a range of values by either
incrementing or decrementing the value.

Some use cases for the WidgetRange widget include volume control, brightness control, time selection, temperature control, etc.

The WidgetRange widget has the following properties:

- **value**: The initial value of the widget.
- **step**: The amount by which the value should be incremented or decremented.
- **min**: The minimum value that the widget can take.
- **max**: The maximum value that the widget can take.
- **format**: The format string used to display the value (default: "%d").
- **cursorOffset**: The offset of the cursor from the end of the widget when the widget is focused (default: 0).
- **cycle**: Whether the selection should cycle back to the beginning when the end of the range is reached (default: false).
- **callback**: A callback function that will be called when the value is changed (default: nullptr).
  The callback is not required if you have multiple widgets in a WidgetItem and you want to handle the selection in the WidgetItem callback.

The following are examples of how to create WidgetRange widgets:

Price picker
------------

.. code-block:: c++

    WIDGET_RANGE(10, 5, 0, 1000, "%.1d€", 1)

In the above example the WidgetRange allows the user to select a price between 0€ and 1000€ in steps of 5€.
The selected price will be displayed as **"10€"**, **"15€"**, **"20€"**, etc with the cursor placed at the end of the value ie. on the '€'.

Time picker
-----------

.. code-block:: c++

    ...
    WIDGET_RANGE(0, 1, 0, 23, "%02d", 0, false),
    WIDGET_RANGE(0, 1, 0, 59, ":%02d", 0, false),
    ...

In the above example the WidgetRange allows the user to select a time between 00:00 and 23:59.
The selected time will be displayed as **"00:00"**, **"00:01"**, **"00:02"**, etc with the cursor placed at the end of the value ie. on the last digit.

Date picker
-----------

.. code-block:: c++

    ...
    WIDGET_RANGE(1, 1, 1, 31, "%02d", 0, false),
    WIDGET_RANGE(1, 1, 1, 12, "/%02d", 0, false),
    WIDGET_RANGE(2021, 1, 2021, 2025, "/%04d", 0, false),
    ...

In the above example the WidgetRange allows the user to select a date between 01/01/2021 and 31/12/2025.
The selected date will be displayed as **"01/01/2021"**, **"01/02/2021"**, **"01/03/2021"**, etc with the cursor placed at the end of the value ie. on the last digit.

Temperature picker (with reference value)
-----------------------------------------

.. code-block:: c++

    int8_t selectedTemperature = 20;

    WIDGET_RANGE_REF(selectedTemperature, 1, 0, 40, "%d°C", 0, false)

In the above example the WidgetRange allows the user to select a temperature between 0°C and 40°C.
The selected temperature value can be updated by changing the value of the **selectedTemperature** variable.
After the value is updated, the WidgetRange will automatically update the selected temperature on the next polling cycle (if :cpp:func:`polling <LcdMenu::poll>` is enabled) or
immediately if you call the :cpp:func:`refresh <LcdMenu::refresh>` function.

For a complete example of using `WIDGET_RANGE_REF`, see the :doc:`use by ref example </reference/samples/UseByRef>`.