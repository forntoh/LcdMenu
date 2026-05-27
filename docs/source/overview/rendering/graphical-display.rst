Graphical display renderer
==========================

``GraphicalDisplayRenderer`` targets pixel-addressable displays through
:cpp:class:`GraphicalDisplayInterface`. The included
``U8g2DisplayAdapter`` lets it render menus on displays supported by
`U8g2 <https://github.com/olikraus/u8g2>`__, including OLED modules such
as SSD1306 and monochrome graphical LCDs such as ST7920.

Unlike HD44780-style character LCD renderers, the graphical renderer draws
with pixels instead of fixed character cells. This gives it enough control to
highlight rows, draw indicators, draw checkboxes and toggles, right-align
values, and flush complete buffered frames to the display.

Features
--------

- U8g2-backed display support through ``U8g2DisplayAdapter``.
- Pixel-addressable rendering instead of fixed-width LCD character cells.
- Dynamic row and column calculation from the active font metrics.
- Row highlighting for the selected item.
- Active value selection highlighting while editing input and charset input
  items.
- Checkbox and toggle rendering for boolean items and toggle commands.
- List and submenu indicators drawn as graphical glyphs.
- Scrollbar rendering when the menu has more items than visible rows.
- Buffered drawing with renderer-managed frame flushing.
- Per-item fonts, plus a default font for items that do not override it.

Layout and fonts
----------------

Rows and columns are calculated from the selected font metrics and the display
pixel size. The renderer measures the active U8g2 font height to determine how
many menu rows fit on the display. It also measures text width to reserve room
for values, cursor icons, list/submenu indicators, checkbox/toggle boxes,
padding, and the scrollbar.

Pass a default font when constructing the renderer or later call
``setDefaultFont()``. Use ``setItemFont()`` or ``ITEM_FONT`` when a specific
menu item should use a different U8g2 font. The renderer reapplies each item's
font before measuring and drawing that row, so larger headings and smaller
detail rows can coexist on the same screen.

Frame buffering
---------------

The renderer implements ``FrameLifecycleRenderer``. During normal menu updates
it clears the U8g2 buffer at the start of a frame, draws all visible rows, and
sends the buffer at the end of the frame. This avoids partially drawn menu
states appearing on buffered displays.

Item rendering
--------------

``GraphicalDisplayRenderer`` uses optional renderer and item capabilities to
draw richer item states while keeping character-display APIs small:

- The focused row is highlighted with an inverted row background.
- The active value or character range is highlighted while editing values,
  input strings, or charset input strings.
- Boolean items and toggle commands render a graphical checkbox/toggle box.
- List items render a list indicator when the value can be cycled.
- Submenu items render a submenu indicator to show that selecting the row opens
  another screen.
- A scrollbar appears when the current screen has hidden rows above or below the
  visible viewport.

Character and glyph support
---------------------------

Graphical displays can often show many more symbols than HD44780-style 16x2
LCDs. With a character LCD, symbols such as ``±`` commonly require registering
custom LCD characters in one of the display's limited custom-character slots.
With U8g2-backed graphical displays, you can usually type the symbol directly
in the label, value format string, or widget format string as long as the
selected U8g2 font contains that glyph.

For example, this format string can render the plus-minus symbol directly on a
graphical display when the active font includes it:

.. code-block:: cpp

    WIDGET_RANGE(10, 1, 0, 100, "±%d%%", 1)

If the selected font does not contain the glyph, choose a U8g2 font that does
or provide a custom font/glyph set. Useful U8g2 references:

- `U8g2 font list <https://github.com/olikraus/u8g2/wiki/fntlistall>`__
- `U8g2 setFont reference <https://github.com/olikraus/u8g2/wiki/u8g2reference#setfont>`__
- `U8g2 font format <https://github.com/olikraus/u8g2/wiki/u8g2fontformat>`__
- `U8g2 bdfconv custom font tool <https://github.com/olikraus/u8g2/wiki/bdfconv>`__

Menu integration
----------------

``MenuScreen`` provides viewport and active-item context to graphical renderers
through ``GraphicalRendererContext``. This lets the renderer recompute visible
rows and value-column widths when fonts differ per item.

Item capabilities
-----------------

Built-in items now expose graphical capabilities through
``GraphicalMenuItem`` without RTTI:

- ``ITEM_BASIC`` and ``ITEM_LABEL`` opt into graphical-item capabilities.
- ``ITEM_BOOL`` and ``ITEM_TOGGLE`` expose toggle state for checkbox drawing.
- Widget-based items (for example ``ITEM_LIST`` and ``ITEM_RANGE``) expose
  list-indicator support.
- ``ITEM_VALUE`` reports the rendered value width for right-aligned layout.

Renderer-specific enhancements remain optional through ``queryExtension()``.
For example, indicators use ``GraphicalIndicatorRenderer`` and value selection
highlighting can be added with ``GraphicalValueSelectionRenderer``.

``GraphicalDisplayRenderer`` now exposes ``GraphicalValueSelectionRenderer`` so
``ItemInput`` and ``ItemInputCharset`` can render active-character selection on
graphical displays.

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
