Item Bool
---------

The :cpp:class:`ItemBool` class is a wrapper around the :cpp:class:`ItemWidget` class that simplifies the creation of menu items with a single :cpp:class:`WidgetBool`. It reduces boilerplate code when you want to create an item with only one widget, making your code cleaner and easier to maintain.

This widget is particularly useful when you need to toggle a boolean value, such as enabling/disabling a feature or switching between two states.

Usage
~~~~~

The :cpp:class:`ItemBool` class allows you to create a menu item that toggles a boolean value. It provides two main functions:

1. **`ITEM_BOOL`**: Creates an item that toggles a boolean value.
2. **`ITEM_BOOL_REF`**: Creates an item that toggles a boolean value, using a reference to track the current value.

Both functions are wrappers around the :cpp:class:`ItemWidget` class with a :cpp:class:`WidgetBool` as the widget.

Example: Simple Toggle
#######################

.. code-block:: c++

    MENU_SCREEN(mainScreen, mainItems,
        ITEM_BOOL("Backlight", false, "On", "Off", [](const bool value) {
            Serial.println(value ? "Backlight On" : "Backlight Off");
        })
    );

In this example, the :cpp:class:`ItemBool` allows the user to toggle the backlight on or off. The selected state is printed to the serial monitor.

Example: Toggle with Reference
###############################

.. code-block:: c++

    bool backlight = false;

    MENU_SCREEN(mainScreen, mainItems,
        ITEM_BOOL_REF("Backlight", backlight, "On", "Off", [](const Ref<bool> value) {
            Serial.println(value.value ? "Backlight On" : "Backlight Off");
        })
    );

In this example, the :cpp:class:`ItemBool` uses a reference to track the current state of the backlight. This is useful when the state needs to be updated externally.

Scenarios
---------

The :cpp:class:`ItemBool` is ideal for scenarios where you need to:

- Toggle a boolean value (e.g., enable/disable a feature).
- Reduce boilerplate code when creating items with a single :cpp:class:`WidgetBool`.
- Dynamically update the state using a reference.

For more details, refer to the :doc:`../widgets/widget-bool` documentation.