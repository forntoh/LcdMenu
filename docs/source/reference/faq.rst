FAQ
===

This is a list of Frequently Asked Questions about |project|. Feel free to suggest new entries!

.. dropdown:: How do I get started with |project|?
    :animate: fade-in-slide-down

    To get started with |project|, follow the steps in the :doc:`Getting Started guide </overview/getting-started>`.

.. dropdown:: How do I create a custom menu item?
    :animate: fade-in-slide-down

    To create a new menu item, you need to extend the base menu item class and implement the necessary methods.
    Check the :doc:`API reference </reference/api/MenuItem>` for more information.

.. dropdown:: How do I update a menu item on runtime?
    :animate: fade-in-slide-down

    To update any attribute of a menu item on runtime, you need to call the appropriate setter method.
    
    For example, to update the text of a basic menu item, you can call the :cpp:func:`MenuItem::setText` method.
    Then call the :cpp:func:`LcdMenu::refresh` method to refresh the screen.

.. dropdown:: How do I hide the menu?
    :animate: fade-in-slide-down

    To hide the menu, you can call the :cpp:func:`LcdMenu::hide` method.
    This will clear the screen and stop the menu from rendering.
    To show the menu again, call the :cpp:func:`LcdMenu::show` method.

You can find extended `discussions <https://github.com/forntoh/LcdMenu/discussions>`_ on the GitHub repository.
