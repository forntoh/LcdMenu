Analog Button Adapter
--------------

The button adapter is a simple input adapter that maps physical buttons connected to analog pins to menu controls.


To use the analog button adapter, you need to include the necessary header:

.. code-block:: cpp

    #include <input/AnalogButtonAdapter.h>


Next, you need to create a AnalogButtonAdapter object and pass the analog Pin, value of valid press, otional margin (default: 20) and the menu object:

.. code-block:: cpp
        
        AnalogButtonAdapter ButtonBackSpace(&menu, A0, 20, 20, BACKSPACE);
        AnalogButtonAdapter ButtonUp(&menu, A0, 100, 20, UP);
        AnalogButtonAdapter ButtonDown(&menu, A0, 250, 20, DOWN);
        AnalogButtonAdapter ButtonBack(&menu, A0, 400, 20, BACK);
        AnalogButtonAdapter ButtonEnter(&menu, A0, 640, 20, ENTER);


Finally, you need to observe the AnalogButtonAdapter object in the loop function:

.. code-block:: cpp

    void loop() {
          ButtonBackSpace.observe();
          ButtonUp.observe();
          ButtonDown.observe();
          ButtonBack.observe();
          ButtonEnter.observe();
    }

The AnalogButtonAdapter will take care of translating the physical button presses into menu controls, allowing you to navigate through the menu system with ease.

For more information about the AnalogButtonAdapter, check the :cpp:class:`API reference <AnalogButtonAdapter>`.
