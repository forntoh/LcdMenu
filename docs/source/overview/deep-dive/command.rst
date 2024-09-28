Command menu item
-----------------

The command menu item is a menu item that executes a function when selected by the user.
It is useful for creating menu items that perform a specific action when selected.

This can be used in various scenarios, such as:

- Controlling a device or sensor (e.g., turning on a motor)
- Changing a setting or configuration (e.g. saving a setting/value)
- Triggering an event or action (e.g., sending a message)

How to create a command menu item
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

A command menu item can be created using the following syntax:

.. tab-set::

    .. tab-item:: As a lambda function

        .. code-block:: cpp

            ITEM_COMMAND("Item 1", []() {
                // Function body
            })

    .. tab-item:: As a function pointer

        .. code-block:: cpp

            // Function definition
            void myFunction() {
                // Function body
            }
            // Create a command menu item
            ITEM_COMMAND("Item 1", myFunction)

Let's take a look at an example of a command menu item that simply prints a message to the serial monitor when selected:

.. code-block:: cpp

    ITEM_COMMAND("Print Message", []() {
        Serial.println("Hello, world!");
    })

When the "Print Message" menu item is selected, the message "Hello, world!" will be printed to the serial monitor.

.. code-block:: console

    $ Hello, world!

Find more information about the command menu item in the :doc:`API reference </reference/api/ItemCommand>`.