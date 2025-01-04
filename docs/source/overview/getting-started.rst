Getting started
===============

.. rst-class:: lead

    A quick guide to get you started with |project|.

|project| is a simple but powerful library for creating menus systems on embedded devices.
It is designed to be easy to use and to work with a wide range of display interfaces and provide
various helpers for handling user input like rotary encoders and buttons.

Installation
------------

Follow `this guide <https://www.ardu-badge.com/LcdMenu>`_ to install the library with **Arduino Library Manager** or install it with **PlatformIO** using the steps below:

1. Open the PlatformIO IDE or VSCode with PlatformIO extension installed.

2. Create a new project or open an existing one.

3. Add the LcdMenu library to your project by adding the following line to your platformio.ini file:

    .. code-block:: bash
    
        lib_deps =
            forntoh/LcdMenu@^5.5.0

4. Save the changes to the platformio.ini file.

5. Build and upload your project to your device.

Usage
-----

To create a menu system with |project|, you need to define a menu structure and a display interface and an input method to interact with the menu.
For this example, we will use the ``LiquidCrystal_I2C`` display interface and ``KeyboardAdapter`` for user input to read from the serial monitor.
You can replace these with other display interfaces and input methods as needed or create your own.

.. code-block:: cpp

    #include <LcdMenu.h>
    #include <MenuScreen.h>
    #include <display/LiquidCrystal_I2CAdapter.h>
    #include <renderer/CharacterDisplayRenderer.h>
    #include <input/KeyboardAdapter.h>

    MENU_SCREEN(mainScreen, mainItems,
        ITEM_BASIC("Item 1"),
        ITEM_BASIC("Item 2"),
        ITEM_BASIC("Item 3"),
        ITEM_BASIC("Item 4"));

    LiquidCrystal_I2C lcd(0x27, 16, 2);
    LiquidCrystal_I2CAdapter lcdAdapter(&lcd);
    CharacterDisplayRenderer renderer(&lcdAdapter, 16, 2);
    LcdMenu menu(renderer);
    KeyboardAdapter keyboard(&menu, &Serial);

    void setup() {
        Serial.begin(9600);
        renderer.begin();
        menu.setScreen(mainScreen);
    }

    void loop() {
        keyboard.observe();
    }

This example creates a simple menu with four items and displays it on a 16x2 LCD screen.
The menu is controlled using the serial monitor, where you can navigate through the items using the arrow keys.

For detailed information on how to use |project|, check out the :doc:`API Reference </reference/api/index>`.

In the next chapters, we will go through the different components of the library and how to use them to create more complex menu systems.
