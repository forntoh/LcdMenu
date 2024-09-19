# Rotary Nav Config

### Overview

The [`RotaryNavConfig`](command:\_github.copilot.openSymbolFromReferences?%5B%22%22%2C%5B%7B%22uri%22%3A%7B%22scheme%22%3A%22file%22%2C%22authority%22%3A%22%22%2C%22path%22%3A%22%2FUsers%2Fforntoh%2FProjects%2FArduino%2FLcdMenu%2Fsrc%2Futils%2FRotaryNavConfig.h%22%2C%22query%22%3A%22%22%2C%22fragment%22%3A%22%22%7D%2C%22pos%22%3A%7B%22line%22%3A15%2C%22character%22%3A7%7D%7D%5D%2C%2238325f39-b010-41db-9117-c9c3f04dc03a%22%5D) helper provides a configuration structure and functions to handle rotary encoder navigation in an LCD menu. This helper allows users to navigate through menu items and input characters using a rotary encoder.

### Basic Usage

#### Setup

1.  **Include the necessary headers:**

    ```cpp
    #include <LcdMenu.h>
    #include <SimpleRotary.h>
    #include <utils/RotaryNavConfig.h>
    ```
2.  **Define the LCD and Rotary Encoder:**

    ```cpp
    #define LCD_ROWS 2
    #define LCD_COLS 16

    LiquidCrystalI2CAdapter lcdAdapter(0x27, LCD_COLS, LCD_ROWS);
    LcdMenu menu(lcdAdapter);

    SimpleRotary encoder(2, 3, 4);
    ```
3.  **Configure the Rotary Navigation:**

    ```cpp
    RotaryNavConfig menuConfig = {
        .encoder = &encoder,
        .menu = &menu,
        .longPressDuration = 1000,
    };
    ```
4.  **Initialize the Menu:**

    ```cpp
    void setup() {
        Serial.begin(9600);
        menu.initialize(mainMenu);
    }
    ```
5.  **Process Rotary Encoder Input:**

    ```cpp
    void loop() {
        processWithRotaryEncoder(&menuConfig);
    }
    ```

#### Actions

* **Rotate Clockwise:** Move to the next menu item.
* **Rotate Counter-Clockwise:** Move to the previous menu item.
* **Short Press:** Select the current menu item.
* **Long Press:** Go back to the previous menu or exit edit mode.

### Charset Input

For advanced usage, you can configure the rotary encoder to input characters from a predefined charset.

#### Setup

1.  **Include the necessary headers:**

    ```cpp
    #include <ItemInput.h>
    #include <LcdMenu.h>
    #include <SimpleRotary.h>
    #include <utils/RotaryNavConfig.h>
    ```
2.  **Define the Charset and Position:**

    ```cpp
    #define CHARSET_SIZE 26
    const char* charset = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int8_t charsetPosition = -1;
    ```
3.  **Configure the Rotary Navigation with Charset:**

    ```cpp
    RotaryNavConfig menuConfig = {
        .encoder = &encoder,
        .menu = &menu,
        .longPressDuration = 1000,
        .charset = charset,
        .charsetSize = CHARSET_SIZE,
        .charsetPosition = charsetPosition,
    };
    ```
4.  **Initialize the Menu:**

    ```cpp
    void setup() {
        Serial.begin(9600);
        menu.initialize(mainMenu);
    }
    ```
5.  **Process Rotary Encoder Input:**

    ```cpp
    void loop() {
        processWithRotaryEncoder(&menuConfig);
    }
    ```

#### Actions

* **Rotate Clockwise:** Move to the next character in the charset.
* **Rotate Counter-Clockwise:** Move to the previous character in the charset.
* **Short Press:** Select the current character.
* **Long Press:** Confirm the input and exit edit mode.

### Examples

{% tabs %}
{% tab title="InputRotary" %}
{% @github-files/github-code-block url="https://github.com/forntoh/LcdMenu/blob/master/examples/InputRotary/InputRotary.ino" %}
{% endtab %}

{% tab title="SimpleRotary" %}
{% @github-files/github-code-block url="https://github.com/forntoh/LcdMenu/blob/master/examples/SimpleRotary/SimpleRotary.ino" %}
{% endtab %}
{% endtabs %}
