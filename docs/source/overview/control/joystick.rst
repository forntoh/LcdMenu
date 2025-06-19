Joystick Adapter
----------------

The joystick adapter translates movements of an analog joystick into
menu commands. It allows navigating the menu with just a simple
analog stick.

The horizontal axis is mapped to ``BACK`` and ``RIGHT`` while the
vertical axis controls ``UP`` and ``DOWN``. The joystick's button can
be handled separately using a :cpp:class:`ButtonAdapter`. All axis
commands can be customised when creating the adapter.

Include the adapter header and create an instance:

.. code-block:: cpp

    #include <Button.h>
    #include <input/ButtonAdapter.h>
    #include <input/JoystickAdapter.h>

    Button pressBtn(2);
    JoystickAdapter joystick(&menu, A0, A1);
    ButtonAdapter pressAdapter(&menu, &pressBtn, ENTER);

``A0`` and ``A1`` are the analog pins for the horizontal and vertical axes.
The joystick button can be handled separately using a
:cpp:class:`ButtonAdapter` as shown. In ``loop()`` simply call
``joystick.observe()`` and ``pressAdapter.observe()``.

.. code-block:: cpp

    void loop() {
        joystick.observe();
        pressAdapter.observe();
    }

The adapter handles the dead zone around the centre value so only
distinct movements generate commands. You can override the default
commands by passing them to the constructor:

.. code-block:: cpp

    // use A2/A3 as axes and map horizontal movement to BACK/ENTER
    JoystickAdapter custom(
        &menu,
        A2,
        A3,
        100,
        BACK,
        ENTER,
        UP,
        DOWN);
