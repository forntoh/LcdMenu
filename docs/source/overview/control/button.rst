Button Adapter
--------------

The button adapter is a simple input adapter that maps physical buttons connected to digital pins to menu controls.

Button Adapter wraps the functionality of the `Button library <https://github.com/madleech/Button>`_ and provides an easy way to connect buttons to the menu system.

To use the button adapter, you need to include the necessary headers:

.. code-block:: cpp

    #include <Button.h>
    #include <input/ButtonAdapter.h>

Then you can create a Button object for each physical button you want to connect to the menu system:

.. code-block:: cpp

    Button upButton(UP_PIN);
    Button downButton(DOWN_PIN);
    Button enterButton(ENTER_PIN);
    Button backButton(BACK_PIN);

Next, you need to create a ButtonAdapter object and pass the Button object and the menu object to it:

.. code-block:: cpp

    ButtonAdapter upButtonA(&menu, &upButton, UP);
    ButtonAdapter downButtonA(&menu, &downButton, DOWN);
    ButtonAdapter enterButtonA(&menu, &enterButton, ENTER);
    ButtonAdapter backButtonA(&menu, &backButton, BACK);


Then you need to call the begin function in the setup function:

.. code-block:: cpp

    void setup() {
        upButton.begin();
        downButton.begin();
        enterButton.begin();
        backButton.begin();
    }

Finally, you need to observe the ButtonAdapter object in the loop function:

.. code-block:: cpp

    void loop() {
        upButtonA.observe();
        downButtonA.observe();
        enterButtonA.observe();
        backButtonA.observe();
    }

The ButtonAdapter will take care of translating the physical button presses into menu controls, allowing you to navigate through the menu system with ease.

For more information about the ButtonAdapter, check the :doc:`API reference </reference/api/input/ButtonAdapter>`.