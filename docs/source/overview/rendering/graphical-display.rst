Graphical display renderer
=========================

The graphical display renderer works with dot-matrix displays using the
:cpp:class:`GraphicalDisplayInterface`. It is ideal for OLED or graphical
LCD modules driven by the `U8g2` library, including classic ST7920 128x64
SPI LCDs.

Features
--------

* Scalable font rendering using the active U8g2 font
* Scroll bar indicator instead of arrow icons
* Customizable cursor characters

Basic usage
-----------

.. code-block:: cpp

#include <U8g2lib.h>
#include <display/U8g2DisplayAdapter.h>
#include <renderer/GraphicalDisplayRenderer.h>

    U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);
U8g2DisplayAdapter lcdAdapter(&u8g2);
GraphicalDisplayRenderer renderer(&lcdAdapter, 6);

void setup() {
    renderer.begin();
}
