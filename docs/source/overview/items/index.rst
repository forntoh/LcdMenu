Item overview
=============

The first thing you need to know is that the system is built around the concept of a menu screen.
A menu screen is a collection of menu items that are displayed on the screen.
Each menu item can contain zero or more widgets and can be interacted with by the user. Widgets provide specialized functionality such as boolean toggles, list selection, and range inputs, allowing menu items to handle various types of user interaction and data display.

|project| provides a number of built-in menu items and widgets that you can use to create your menu structure.

.. toctree::
    :maxdepth: 2
    :caption: Here are some of the built-in menu items:

    basic
    command
    submenu
    list
    range
    input
    input-charset

You can also create your own custom menu items and widgets by extending the base menu item class or any of the existing menu items. See the :doc:`../widgets/index` section for more information about available widgets and their usage.