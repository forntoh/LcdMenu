Rotary Encoder Adapter
----------------------

The rotary encoder adapter is an input adapter that maps rotary encoders to menu controls.

Rotary encoders are input devices that allow you to rotate a knob to adjust values or navigate through a menu.

Rotary Encoder Adapter wraps the functionality of the `SimpleRotary library <https://www.arduinolibraries.info/libraries/simple-rotary>`_
and provides an easy way to connect rotary encoders to the menu system.

The actions that are mapped on the rotary encoder are:

- **UP**: Rotate the encoder clockwise.
- **DOWN**: Rotate the encoder counterclockwise.
- **ENTER**: Press the encoder button.
- **BACK**: Press the encoder button for a long time.
- **BACKSPACE**: Press the encoder button twice. This action can be disabled if you don't need it or prefer to use
  a dedicated button. See this :doc:`example </reference/samples/ButtonWithRotaryAdapter>` for more information.

To use the rotary encoder adapter, you need to include the necessary headers:

.. code-block:: cpp

    #include <SimpleRotary.h>
    #include <input/SimpleRotaryAdapter.h>

Then you can create a SimpleRotary object for each rotary encoder you want to connect to the menu system:

.. code-block:: cpp

    SimpleRotary encoder(ENCODER_PIN_A, ENCODER_PIN_B, ENCODER_PIN_SW);

Next, you need to create a SimpleRotaryAdapter object and pass the SimpleRotary object and the menu object to it:

.. code-block:: cpp

    SimpleRotaryAdapter encoderA(&menu, &encoder);

Finally, you need to observe the SimpleRotaryAdapter object in the loop function:

.. code-block:: cpp

    void loop() {
        encoderA.observe();
    }

The ``SimpleRotaryAdapter`` will take care of translating the rotary encoder movements into menu controls, allowing you to navigate through the menu system with ease.

For more information about the ``SimpleRotaryAdapter``, check the :cpp:class:`API reference <SimpleRotaryAdapter>`.