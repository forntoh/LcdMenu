Controlling the menu
====================

A menu system without control is like a car without a driver.
You need to be able to navigate through the menu structure, select items, and perform actions.
The menu system provides a set of built-in controls that you can use to interact with the menu items.

Here are some of the built-in controls that you can use to navigate through the menu system:

- **UP/DOWN**: Move the selection up or down in the menu.
- **LEFT/RIGHT**: Mainly used for adjusting values in input items.
- **ENTER**: Select the currently highlighted menu item.
- **BACK**: Navigate back to the previous menu screen.
- **BACKSPACE**: Delete the selected character in an input item.
- **CLEAR**: Clear the input value in an input item.

**UP/DOWN** and **ENTER** are the most commonly used controls, as they allow you to navigate through the menu structure and select items.
The other controls are used in specific scenarios, such as when you need to adjust a value in an input item or delete characters in an input field.
The back control can be bound to a menu item, allowing you to navigate back to the previous menu screen without having to use a dedicated "Back" button.

You can perform an action on the menu by calling the :cpp:func:`LcdMenu::process` method on the menu object and pass the command you want to execute.
For example, to move the selection down in the menu, you can call ``menu.process(DOWN)``.

Luckily, you don't have to worry about implementing these controls yourself.
The menu system takes care of handling the controls and updating the menu state accordingly with the help of input adapters.
Input adapters are responsible for translating the physical input from the user into control commands that the menu system can understand.

The menu system provides a set of built-in input adapters that you can use out of the box.

.. toctree::
    :maxdepth: 1
    :caption: Here are some of the built-in input adapters that you can use to interact with the menu system:

    keyboard
    button
    rotary-encoder
    analogButton
    joystick

.. hint::
    The left and right controls (if not already mapped by the item) is mapped to scroll through an item that is too long to fit on the screen.

    .. image:: images/view-shift.gif
        :width: 400px
        :alt: Scrolling through a long item
