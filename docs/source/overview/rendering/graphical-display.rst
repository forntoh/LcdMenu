Graphical display renderer
==========================

The graphical display renderer targets pixel-addressable displays through
:cpp:class:`GraphicalDisplayInterface`.

It is designed for U8g2-driven modules, including OLED displays such as
SSD1306 and monochrome graphical LCDs such as ST7920.

Features
--------

- Dynamic row and column calculation from the active font metrics
- Row highlighting for selection and value-area highlighting while editing
- Checkbox rendering for boolean items and toggles
- Scrollbar and value indicators for widget-driven items
- Buffered drawing with renderer-managed frame flushing
- Per-item custom fonts (any U8g2 font)

Menu integration
----------------

``MenuScreen`` provides viewport and active-item context to graphical renderers
through ``GraphicalRendererContext``. This lets the renderer recompute visible
rows and value-column widths when fonts differ per item.

Basic usage
-----------

.. code-block:: cpp

    #include <U8g2lib.h>
    #include <display/U8g2DisplayAdapter.h>
    #include <renderer/GraphicalDisplayRenderer.h>

    U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);
    U8g2DisplayAdapter display(&u8g2);
    GraphicalDisplayRenderer renderer(&display, u8g2_font_6x10_tf);

    MENU_SCREEN(mainScreen, mainItems,
        ITEM_FONT(ITEM_LABEL("Overview"), u8g2_font_7x13B_tf),
        ITEM_BOOL("Enabled", true, "ON", "OFF", nullptr));

    void setup() {
        renderer.begin();
        renderer.setItemFont(mainItems[1], u8g2_font_6x13_tf);
    }
