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