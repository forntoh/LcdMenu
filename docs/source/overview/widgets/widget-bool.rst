WidgetBool
==========

The WidgetBool widget acts as a toggle switch that can be used to change a boolean value.

The WidgetBool widget has the following properties:

- **value**: The boolean value that the widget represents.
- **textOn**: The text to display when the value is true.
- **textOff**: The text to display when the value is false.
- **format**: The format to use when displaying the value.
- **cursorOffset**: The offset of the cursor from the end of the widget when the widget is focused.

The following is an example of how to create a WidgetBool widget:

The WIDGET_BOOL macro provides a convenient way to create a WidgetBool instance. The macro takes the following arguments in order: initial value, text for true state, text for false state, format string, and cursor offset.

.. code-block:: c++

    WIDGET_BOOL(false, "Yes", "No", "%s sir", 5)
In the above example, the WidgetBool widget is created with the following properties:

- The initial value is false.
- The text to display when the value is true is "Yes".
- The text to display when the value is false is "No".
- The format to use when displaying the value is "%s sir" (the value will be displayed as "Yes sir" or "No sir").
- The cursor offset is 5 (the cursor will be 5 characters from the end of the widget ie. on the 's' in "Yes" or on the 'o' in "No").

There's also a ``WIDGET_BOOL_REF`` macro that can be used to create a WidgetBool instance with a reference to a boolean value.
The boolean value can be updated by changing the value of the referenced variable.
After the value is updated, the WidgetBool will automatically update its value on the next polling cycle (if :cpp:func:`polling <LcdMenu::poll>` is enabled) or
immediately if you call the :cpp:func:`refresh <LcdMenu::refresh>` function.

.. note::

  The referenced variable must remain valid for the widget’s lifetime to ensure proper and predictable updates.
  Invalidating the referenced variable (e.g., by going out of scope or being deleted) can lead to undefined behavior.
  Ensure that the variable's lifetime exceeds or matches the widget's lifetime to avoid such issues.

For a complete example of using ``WIDGET_BOOL_REF``, see the :doc:`use by ref example </reference/samples/UseByRef>`.

Widget Interaction
------------------

The WidgetBool widget can be toggled using the following controls:
- Press LEFT/RIGHT to toggle between true and false states
- Press ENTER to confirm the current value and:
  - Move to the next widget if available
  - Exit focus mode if no next widget exists