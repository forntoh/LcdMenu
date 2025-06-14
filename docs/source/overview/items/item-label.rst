Item label
----------

The ``ItemLabel`` class represents an unselectable item that can be used as a title or separator within a menu screen. It is useful when you want to display static text without allowing the cursor to stop on it.

Usage
~~~~~

Create a label item using ``ITEM_LABEL``:

.. code-block:: cpp

    MENU_SCREEN(mainScreen, mainItems,
        ITEM_LABEL("Main Menu"),
        ITEM_BASIC("First"),
        ITEM_BASIC("Second"));

When ``mainScreen`` is displayed, the cursor will automatically skip the label and land on the first selectable item.

.. note::
   If you place two or more ``ItemLabel`` instances sequentially, items beyond
   the first may never become visible because the cursor cannot stop on these
   labels to scroll the view.

Refer to the :cpp:class:`ItemLabel` API reference for more details.
