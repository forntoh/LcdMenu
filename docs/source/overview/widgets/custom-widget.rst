Custom Widgets
==============

The built-in widgets display their value with a printf-style format string (for example ``"%d"`` or ``" on %s"``).
Sometimes that is not enough: you may want to derive the displayed text from a function, look it up in your own tables, or format several state variables at once.

For such cases the library provides an escape hatch that requires no library changes: every widget renders itself through a ``protected`` virtual ``draw`` method, so you can subclass an existing widget and override only how it displays its value.

How it works
------------

Every widget inherits from :cpp:class:`BaseWidget`, which declares the method that turns the widget's state into display text:

``uint8_t draw(char* buffer, const uint8_t start = 0)``

When a menu item is rendered, it draws each of its widgets into a shared character buffer:

- **buffer**: the buffer to draw into. It is ``ITEM_DRAW_BUFFER_SIZE`` (64) bytes long.
- **start**: the index in the buffer where this widget must start writing. An item hosting several widgets draws them one after another into the same buffer.
- **return value**: the number of characters written into the buffer. The item uses it to place the next widget on the same line.

The built-in widgets implement this method with ``snprintf`` and the widget's ``format`` string:

- :cpp:class:`BaseWidgetValue` formats the raw value.
- :cpp:class:`WidgetRange` formats the current range value.
- :cpp:class:`WidgetList` formats the **selected list entry** (not the raw index).

To create a custom widget:

1. Subclass an existing widget (:cpp:class:`WidgetRange`, :cpp:class:`WidgetList`, :cpp:class:`WidgetBool`) or :cpp:class:`BaseWidgetValue` directly. Subclassing an existing widget keeps all of its editing behavior (UP/DOWN handling, range clamping, cycling, cancel/restore on BACK) and lets you change only the display.
2. Override the ``protected`` virtual ``draw`` method. Write your display text into ``buffer`` starting at ``start``, never write past ``ITEM_DRAW_BUFFER_SIZE``, and return the number of characters written.
3. Compose the widget into a menu item with ``ITEM_WIDGET`` instead of the ``ITEM_RANGE``/``ITEM_LIST`` convenience macros. ``ITEM_WIDGET`` accepts any ``BaseWidgetValue<T>*``, including pointers to your own subclasses.

Example
-------

The following example shows a custom range widget whose value is displayed using a ``stringForValue`` function instead of a printf format string.
The value is still adjusted between 0 and 100 in steps of 1 (all inherited from :cpp:class:`WidgetRange`), but the widget displays **"Low"**, **"Medium"** or **"High"**:

.. code-block:: c++

    #include <ItemWidget.h>
    #include <LcdMenu.h>
    #include <MenuScreen.h>
    #include <display/LiquidCrystal_I2CAdapter.h>
    #include <input/KeyboardAdapter.h>
    #include <renderer/CharacterDisplayRenderer.h>
    #include <widget/WidgetRange.h>

    #define LCD_ROWS 2
    #define LCD_COLS 16
    #define LCD_ADDR 0x27

    // The function that decides how the value is displayed.
    const char* stringForValue(const int& value) {
        if (value < 34) return "Low";
        if (value < 67) return "Medium";
        return "High";
    }

    // A range widget that draws its value with stringForValue()
    // instead of a printf format string.
    class WidgetLevel : public WidgetRange<int> {
      public:
        WidgetLevel(
            const int value,
            const int step,
            const int min,
            const int max,
            const uint8_t cursorOffset = 0,
            const bool cycle = false,
            void (*callback)(const int&) = nullptr)
            : WidgetRange<int>(value, step, min, max, "%d", cursorOffset, cycle, callback) {}

      protected:
        uint8_t draw(char* buffer, const uint8_t start) override {
            if (start >= ITEM_DRAW_BUFFER_SIZE) return 0;
            return snprintf(buffer + start, ITEM_DRAW_BUFFER_SIZE - start, "%s", stringForValue(this->value));
        }
    };

    MENU_SCREEN(
        mainScreen,
        mainItems,
        ITEM_WIDGET(
            "Level",
            [](int level) { Serial.println(level); },
            new WidgetLevel(0, 1, 0, 100)));

    LiquidCrystal_I2C lcd(LCD_ADDR, LCD_COLS, LCD_ROWS);
    LiquidCrystal_I2CAdapter lcdAdapter(&lcd);
    CharacterDisplayRenderer renderer(&lcdAdapter, LCD_COLS, LCD_ROWS);
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

In the above example the item displays **"Low"** for values below 34, **"Medium"** for values below 67 and **"High"** otherwise, as determined by ``stringForValue``.
The ItemWidget callback receives the widget's actual value (an ``int``), not the displayed text.

Notes:

- The override should stay in a ``protected`` section of your subclass, matching how the built-in widgets declare ``draw`` (it is declared ``protected`` in :cpp:class:`BaseWidget`).
- Always respect the ``start`` offset and the ``ITEM_DRAW_BUFFER_SIZE`` limit, and return the number of characters written, exactly like the built-in widgets do.
- The ``format`` string is still required by the :cpp:class:`WidgetRange` constructor, but it is ignored by the custom ``draw`` method.
- Custom widgets are composed with ``ITEM_WIDGET`` (see the :doc:`ItemWidget </overview/items/item-widget>` page); the behavior you inherit comes from :doc:`WidgetRange <widget-range>`, :doc:`WidgetList <widget-list>` and :doc:`WidgetBool <widget-bool>`.

Find more information about the base classes in the :cpp:class:`API reference <BaseWidgetValue>`.
