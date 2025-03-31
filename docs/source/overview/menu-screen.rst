Menu Screen
===========

The :cpp:class:`MenuScreen` class is the foundation of the |project| library's menu system. It represents a single screen in the menu hierarchy and
is responsible for managing a collection of menu items (:cpp:class:`MenuItem`), handling navigation, and rendering the menu on the display.

With :cpp:class:`MenuScreen`, you can create dynamic, hierarchical menus that allow users to interact with your embedded application in an intuitive way.
Whether you're building a simple menu or a complex multi-level menu system, :cpp:class:`MenuScreen` provides the tools you need.

Why Use MenuScreen?
-------------------

The :cpp:class:`MenuScreen` class simplifies the process of creating and managing menus in embedded systems. It provides:

1. **Dynamic Menu Management**:
   - Add, remove, or update menu items at runtime.
   - Create menus that adapt to user input or application state.

2. **Hierarchical Navigation**:
   - Easily create submenus and navigate between them using the ``BACK`` and ``RIGHT`` commands.
   - Set parent screens for seamless navigation.

3. **Cursor and View Management**:
   - Automatically handles cursor movement and ensures only the visible portion of the menu is displayed.

4. **Integration with Renderers**:
   - Works seamlessly with :cpp:class:`MenuRenderer` to display menus on various types of displays.

5. **Polling for Updates**:
   - Supports periodic updates for menu items that require dynamic behavior, such as real-time data or animations.

How to Use MenuScreen
---------------------

Creating a MenuScreen
^^^^^^^^^^^^^^^^^^^^^^

To create a :cpp:class:`MenuScreen`, you define a list of :cpp:class:`MenuItem` objects and pass them to the :cpp:class:`MenuScreen` constructor.
The ``MENU_SCREEN`` macro simplifies this process.

.. code-block:: cpp

    MENU_SCREEN(mainScreen, mainItems,
        new MenuItem("Option 1"),
        new MenuItem("Option 2"),
        new MenuItem("Option 3")
    );

In this example:
- A :cpp:class:`MenuScreen` named ``mainScreen`` is created.
- It contains three menu items: "Option 1", "Option 2", and "Option 3".

Navigating Between Screens
^^^^^^^^^^^^^^^^^^^^^^^^^^^

You can create hierarchical menus by linking screens together. For example, you can set a parent screen for a submenu:

.. code-block:: cpp

    MENU_SCREEN(subScreen, subItems,
        new MenuItem("Sub Option 1"),
        new MenuItem("Sub Option 2")
    );

    subScreen->setParent(mainScreen);

When the user navigates back from ``subScreen``, they will return to ``mainScreen``.

Adding and Removing Items Dynamically
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

You can modify the menu at runtime by adding or removing items:

.. code-block:: cpp

    mainScreen->addItem(new MenuItem("New Option"));
    mainScreen->removeItemAt(1);  // Removes the second item

This is useful for creating menus that adapt to the application's state or user input.

Polling for Updates
^^^^^^^^^^^^^^^^^^^

If your menu items require periodic updates (e.g., displaying real-time data), you can use the :cpp:func:`poll <LcdMenu::poll>` method:

.. code-block:: cpp

    lcd->poll(1000);  // Poll every 1000ms

This ensures that the menu is updated at regular intervals.

Examples
--------

For complete examples of using :cpp:class:`MenuScreen`, see the following:

- :doc:`Dynamic Menu Example </reference/samples/DynamicMenu>`: Demonstrates how to dynamically add, remove, and update menu items at runtime.
- :doc:`Submenu Example </reference/samples/SubMenu>`: Shows how to create hierarchical menus with parent-child relationships.
- :doc:`Polling Example </reference/samples/UseByRef>` and :doc:`Polling Example </reference/samples/ItemValue>`: Explains how to use the `poll` method for dynamic updates.

Notes
-----

- Ensure that the :cpp:class:`MenuItem` objects passed to a :cpp:class:`MenuScreen` remain valid for the lifetime of the screen.
- Use the `poll` method for menu items that require dynamic updates, such as real-time data or animations.
- The :cpp:class:`MenuScreen` class is designed to work seamlessly with the :cpp:class:`LcdMenu` and :cpp:class:`MenuRenderer` classes for rendering and input handling.

Summary
-------

The :cpp:class:`MenuScreen` class is a powerful tool for creating dynamic, hierarchical menus in embedded systems.
By combining :cpp:class:`MenuScreen` with other components of the |project| library, you can build intuitive and flexible user interfaces for your applications.

For more details, refer to the :doc:`API Reference </reference/api/MenuScreen>` or explore the examples linked above.