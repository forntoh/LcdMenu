---
description: >-
  This example will show you how to display integer values that are bound to a
  given range.
---

# Int & Float Values

Using ItemProgress, you can display integer values in a given range and define the step length between each value.

## Includes

<pre class="language-cpp"><code class="lang-cpp"><strong>// other includes ...
</strong><strong>#include &#x3C;ItemProgress.h>
</strong>#include &#x3C;LcdMenu.h> // Always comes after every item type import
</code></pre>

## Constructors

ItemProgress has multiple constructors which will enable you to customize the base functionality, using the base constructor

```cpp
ItemProgress(const char* key, fptrInt callback)
```

will display an integer value from **0** to **1000** and with a step length of **1** i.e. each time **`left()`** or **`right()`** is called, it will increment and decrement the value by exactly **1.**

To customize the range, use the following constructors which provide a method for you to map the value from **0** to **1000** to any other range (float, int, or long).

```cpp
ItemProgress(const char* key, uint8_t stepLength, fptrMapping mapping, fptrInt callback)
```

* **key**: Text to display before the value.
* **stepLength**: The length of each step for incrementing or decrementing the progress value.
* **mapping**: A pointer to the mapping function that maps the progress value to a custom representation (in this mapping function you can also append any text to the progress).
* **callback**: function to be executed after changing the value and leaving edit mode.

{% hint style="warning" %}
The value passed to the callback function is the progress, so you also have to map it to get your desired value.
{% endhint %}

## Mapping to different range example functions

The following example mapping functions shows how to map the progress value in the range **0..1000** to any other range

{% tabs %}
{% tab title="Long" %}
Map from **1..1000** to **100..200** and add a unit

```cpp
char* intMapping(uint16_t progress) {
    // Map the progress value to a new range (100 to 200)
    long mapped = mapProgress(progress, 100L, 200L);

    // Buffer to store the converted string
    static char buffer[10];

    // Convert the mapped value to a string
    itoa(mapped, buffer, 10);

    // Concatenate the string with the character 'm'
    concat(buffer, 'm', buffer);

    // Return the resulting string
    return buffer;
}
```

{% hint style="info" %}
As you have seen before the default step length is **1**, and there are **100** values between **100** and **200**, so for it to step by **exactly one on the screen** you need to provide a step length of **10** which is **1000/100** (max progress/total values)
{% endhint %}
{% endtab %}

{% tab title="Float" %}
Map from **1..1000** to **-1.0f..1.0f** and add a unit

```cpp
char* floatMapping(uint16_t progress) {
    // Normalize the progress value and map it to the specified floating-point
    // range
    float floatValue = mapProgress(progress, -1.0f, 1.0f);

    // Buffer to store the converted string
    static char buffer[10];

    // Convert the floating-point value to a string with 4 characters (including
    // decimal point) and 2 decimal places
    dtostrf(floatValue, 4, 2, buffer);

    // Concatenate the string with the character 'c'
    concat(buffer, 'c', buffer);

    // Return the resulting string
    return buffer;
}
```

{% hint style="info" %}
As you have seen before the default step length is **1**, and there are **200** values between **-1.0** and **1.0 if you consider a precision of two**, so for it to step by **exactly 0.01 on the screen,** you need to provide a step length of **5** which is **1000/200** (max progress/total values)
{% endhint %}
{% endtab %}
{% endtabs %}

## Code example

```cpp
// Declare the calbacks
void callback(uint16_t pos);

char* intMapping(uint16_t progress) {
    // See code in the above section
}

// Initialize the main menu items
MAIN_MENU(
    ITEM_BASIC("Con"),
    ITEM_PROGRESS("Dist", 1, intMapping, callback),
    ITEM_BASIC("Blink SOS"),
    ITEM_BASIC("Blink random")
);

// Construct the LcdMenu
LiquidCrystalI2CAdapter lcdAdapter(0x27, LCD_COLS, LCD_ROWS);
LcdMenu menu(lcdAdapter);

void setup() {
    Serial.begin(9600);
    // Initialize LcdMenu with the menu items
    menu.initialize(mainMenu);
}

void loop() {
    if (!Serial.available()) return;
    char command = Serial.read();
    processMenuCommand(menu, command, UP, DOWN, ENTER, BACK, LEFT, RIGHT);
}

void callback(uint16_t pos) {
    // do something with the progress
    Serial.println(pos);
}
```

Full example 👉 [.../examples/IntFloatValues/IntFloatValues.ino](https://github.com/forntoh/LcdMenu/tree/master/examples/IntFloatValues/IntFloatValues.ino)
