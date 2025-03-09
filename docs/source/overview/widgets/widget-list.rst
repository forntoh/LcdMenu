WidgetList
==========

The WidgetList widget enables users to select an item from a list of options.
The list of options can be defined by the user and can contain up to 255 items.

Using the WidgetList widget, you can create menu items that present numbers, text, or other settings as selectable options to users.

The WidgetList accepts any type of data as the list items, as long as it can be converted to a string.

WidgetList has the following properties:

- **values**: The list of values to choose from.
- **size**: The number of items in the list.
- **activePosition**: The index of the currently selected item (default: 0).
- **format**: The format string used to display the selected item (default: "%s").
- **cursorOffset**: The offset of the cursor from the end of the widget when the widget is focused (default: 0).
- **cycle**: Whether the selection should cycle back to the beginning when the end of the list is reached (default: false).
- **callback**: A callback function that will be called when an item is selected (default: nullptr).
  The callback is not required if you have multiple widgets in a WidgetItem and you want to handle the selection in the WidgetItem callback.

The following are examples of how to create WidgetList widgets:

Simple list
-----------

.. code-block:: c++

    const char* listItems[] = { "Item 1", "Item 2", "Item 3", "Item 4" };

    WIDGET_LIST(listItems, sizeof(listItems) / sizeof(listItems[0]), 0, "%s", 0, false, [](char* item) {
        // Callback function to handle item selection
    })

In the above example the WidgetList allows the user to select an item from "Item 1", "Item 2", "Item 3", or "Item 4".

Number picker
-------------

.. code-block:: c++

    uint8_t listItems[] = { 7, 2, 9, 4 };

    WIDGET_LIST(listItems, sizeof(listItems) / sizeof(listItems[0]), 0, "%d", 0, false)

In the above example the WidgetList allows the user to select an item from 7, 2, 9, or 4 only.
This is useful when you want to limit the user to a specific set of values.

Day picker
----------

.. code-block:: c++

    const char* days[] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};

    WIDGET_LIST(days, sizeof(days) / sizeof(days[0]), 0, " on %s", 0, true)

In the above example the WidgetList allows the user to select a day of the week.
The selected day will be displayed as **" on Mon"**, **" on Tue"**, **" on Wed"**, etc.

Month picker (with reference value)
-----------------------------------

.. code-block:: c++

    uint8_t selectedMonth = 0;

    const char* months[] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

    WIDGET_LIST_REF(months, sizeof(months) / sizeof(months[0]), selectedMonth, "%s", 0, true)

In the above example the WidgetList allows the user to select a month.
The selected month value can be updated by changing the value of the **selectedMonth** variable.
After the value is updated, the WidgetList will automatically update the selected item on the next polling cycle (if :cpp:func:`polling <LcdMenu::poll>` is enabled) or
immediately if you call the :cpp:func:`refresh <LcdMenu::refresh>` function.

For a complete example of using `WIDGET_LIST_REF`, see the :doc:`use by ref example </reference/samples/UseByRef>`.