WidgetList
==========

The WidgetList widget is a widget that allows the user to select an item from a list of options.
The list of options can be defined by the user and can contain up to 255 items.

Using the WidgetList widget, you can create a menu item that presents a numbers, text, or other settings to the user, allowing them to select one of the items.

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

.. code-block:: c++

    const char* listItems[] = { "Item 1", "Item 2", "Item 3", "Item 4" };

    WIDGET_LIST(listItems, sizeof(listItems) / sizeof(listItems[0]), 0, "%s", 0, false, [](char* item) {
        // Callback function to handle item selection
    })

In the above example the WidgetList allows the user to select an item from "Item 1", "Item 2", "Item 3", or "Item 4".

.. code-block:: c++

    uint8_t listItems[] = { 7, 2, 9, 4 };

    WIDGET_LIST(listItems, sizeof(listItems) / sizeof(listItems[0]), 0, "%d", 0, false)

In the above example the WidgetList allows the user to select an item from 7, 2, 9, or 4 only.
This is useful when you want to limit the user to a specific set of values.

.. code-block:: c++

    const char* days[] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};

    WIDGET_LIST(days, sizeof(days) / sizeof(days[0]), 0, " on %s", 0, true)

In the above example the WidgetList allows the user to select a day of the week.
The selected day will be displayed as **" on Mon"**, **" on Tue"**, **" on Wed"**, etc.