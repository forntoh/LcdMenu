Keyboard Adapter
----------------

The keyboard adapter is an input adapter that maps keyboard inputs to menu controls through the serial port.

The keyboard adapter allows you to interact with the menu system using your computer keyboard.
This can be useful for debugging or testing the menu system without having to connect physical buttons or rotary encoders.

The keyboard adapter maps the following keys to menu controls:

- **UP/DOWN/LEFT/RIGHT**: Arrow keys.
- **ENTER**: :kbd:`Enter` or :kbd:`return`
- **BACK**: :kbd:`esc`
- **BACKSPACE**: :kbd:`backspace`
- **CLEAR**: :kbd:`delete`

To use the keyboard adapter, you need to include the necessary headers:

.. code-block:: cpp

    #include <input/KeyboardAdapter.h>

Then you need to create a KeyboardAdapter object and pass the menu object to it:

.. code-block:: cpp

    KeyboardAdapter keyboard(&menu);

In the setup function, you need to initialize the serial port:

.. code-block:: cpp

    void setup() {
        Serial.begin(9600);
    }

Finally, you need to observe the KeyboardAdapter object in the loop function:

.. code-block:: cpp

    void loop() {
        keyboard.observe();
    }

The ``KeyboardAdapter`` will take care of translating the keyboard inputs into menu controls, allowing you to navigate through the menu system with ease.

For more information about the ``KeyboardAdapter``, check the :cpp:class:`API reference <KeyboardAdapter>`.