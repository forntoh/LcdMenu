Character display renderer
==========================

The character display renderer is a simple renderer that displays the menu items on a character display.
It is a good choice if you are using a character display with your project.

The renderer supports the following features:

- Displaying menu items on a character display
- Truncating long menu items
- Highlighting the selected menu item
- Customizing the cursor character
- Customizing the up and down arrow characters
- Disabling the cursor and/or arrows

The renderer is easy to use and provides a number of options for customizing the display.

How to use the character display renderer
-----------------------------------------

To use the character display renderer, you need to create an instance of the :cpp:class:`CharacterDisplayRenderer` class and pass it
to the :cpp:class:`LcdMenu` class when you create it. Here is an example:

.. tab-set::
    :sync-group: display

    .. tab-item:: Liquid Crystal I2C
         :sync: lcd_i2c

         .. code-block:: cpp

            #include <LcdMenu.h>
            #include <display/LiquidCrystal_I2CAdapter.h>
            #include <renderer/CharacterDisplayRenderer.h>

            LiquidCrystal_I2C lcd(0x27, LCD_COLS, LCD_ROWS);
            LiquidCrystal_I2CAdapter lcdAdater(&lcd);
            CharacterDisplayRenderer renderer(&lcdAdater, LCD_COLS, LCD_ROWS);
            LcdMenu menu(renderer);

    .. tab-item:: Liquid Crystal
         :sync: lcd

         .. code-block:: cpp

            #include <LcdMenu.h>
            #include <display/LiquidCrystalAdapter.h>
            #include <renderer/CharacterDisplayRenderer.h>

            LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
            LiquidCrystalAdapter lcdAdapter(&lcd, LCD_COLS, LCD_ROWS);
            CharacterDisplayRenderer renderer(&lcdAdapter, LCD_COLS, LCD_ROWS);
            LcdMenu menu(renderer);

    .. tab-item:: SSD1803A I2C
         :sync: ssd1803a_i2c

         .. code-block:: cpp

            #include <LcdMenu.h>
            #include <display/SSD1803A_I2CAdapter.h>
            #include <renderer/CharacterDisplayRenderer.h>

            SSD1803A_I2C lcd(0x3D);
            SSD1803A_I2CAdapter lcdAdapter(&lcd);
            CharacterDisplayRenderer renderer(&lcdAdapter, 20, 4);
            LcdMenu menu(renderer);

Don't forget to replace ``LCD_COLS`` and ``LCD_ROWS`` with the number of columns and rows on your display.

After you have created the renderer, you then have to call the ``begin()`` method on the renderer to initialize it.


.. tab-set::
    :sync-group: display

    .. tab-item:: Liquid Crystal I2C
         :sync: lcd_i2c

         .. code-block:: cpp

            void setup() {
                renderer.begin();
            }

    .. tab-item:: Liquid Crystal
         :sync: lcd

         .. code-block:: cpp

            void setup() {
                renderer.begin();
            }

    .. tab-item:: SSD1803A I2C
         :sync: ssd1803a_i2c

         .. code-block:: cpp

            void setup() {
                lcd.begin(DOGM204);
                renderer.begin();
            }

Customizing the character display renderer
------------------------------------------

The character display renderer provides a number of options for customizing the display. You can set the following options:

- The cursor characters (normal and editing)
- The up and down arrow characters
- Whether to display the cursor and/or arrows

Here is an example of how to customize the renderer:

Use custom cursor characters
^^^^^^^^^^^^^^^^^^^^^^^^^^^

You can set custom cursor characters to use when rendering the selected menu item.

.. code-block:: cpp

    CharacterDisplayRenderer renderer(
        new LiquidCrystal_I2CAdapter(&lcd),
        LCD_COLS,
        LCD_ROWS,
        0xA5,  // cursorChar
        0x5F   // editCursorChar
    );

.. image:: images/custom-cursor.gif
    :width: 400px
    :alt: Custom cursor characters

.. note::

    Please refer to the datasheet of your display to find the character codes for the custom cursor characters.


Hide the arrows only
^^^^^^^^^^^^^^^^^^^^

You may want to hide the up and down arrows if you don't need them or want more space for rendering the menu items.
Set the up and down arrow characters to ``NULL`` to hide them.

.. code-block:: cpp

    CharacterDisplayRenderer renderer(
        &lcdAdapter,
        LCD_COLS,
        LCD_ROWS,
        0x7E,  // cursorChar
        0x7F,  // editCursorChar
        NULL,  // upArrow
        NULL   // downArrow
    );

.. image:: images/no-arrows.png
    :width: 400px
    :alt: Hide the arrows

Hide the cursor only
^^^^^^^^^^^^^^^^^^^^

Set the cursor characters to ``0`` to hide the cursor.

.. code-block:: cpp

    CharacterDisplayRenderer renderer(
        &lcdAdapter,
        LCD_COLS,
        LCD_ROWS,
        0,  // cursorChar
        0   // editCursorChar
    );

.. image:: images/no-cursor.webp
    :width: 400px
    :alt: Hide the cursor

Hide both the cursor and arrows
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. code-block:: cpp

    CharacterDisplayRenderer renderer(
        &lcdAdapter,
        LCD_COLS,
        LCD_ROWS,
        0,  // cursorChar
        0,  // editCursorChar
        NULL,  // upArrow
        NULL   // downArrow
    );

.. image:: images/no-cursor-no-arrows.webp
    :width: 400px
    :alt: Hide both the cursor and arrows

If these options are not enough for you, you can always create your own custom renderer by subclassing the :cpp:class:`CharacterDisplayRenderer` class.

Here is basic example of how to create a custom renderer:

.. code-block:: cpp

    class MyCustomRenderer : public CharacterDisplayRenderer {
    public:
        MyCustomRenderer(CharacterDisplayInterface *display, uint8_t cols, uint8_t rows)
            : CharacterDisplayRenderer(display, cols, rows) {
        }

        void drawItem(const char* text, const char * value) override {
            // Custom rendering code here
            // 
            // You can append a cursor character to the text if the item is selected etc.
        }
    };

Find more information about the character display renderer in the :cpp:class:`API reference <CharacterDisplayRenderer>`.
