# Simple Nav Config

### Overview

`SimpleNavConfig` is a helper structure designed to facilitate navigation and control of the `LcdMenu` library. It allows users to define key bindings for menu navigation and supports both basic menu operations and character set input.

### Basic Usage

#### Setup

1.  **Include Necessary Headers:**

    ```cpp
    #include <LcdMenu.h>
    #include <interface/LiquidCrystalI2CAdapter.h>
    #include <utils/SimpleNavConfig.h>
    ```
2.  **Define Menu Items:**

    ```cpp
    MAIN_MENU(
        ITEM_BASIC("Start service"),
        ITEM_BASIC("Connect to WiFi"),
        ITEM_BASIC("Settings"),
        ITEM_BASIC("Blink SOS"),
        ITEM_BASIC("Blink random"));
    ```
3.  **Initialize Display Adapter and Menu:**

    ```cpp
    LiquidCrystalI2CAdapter lcdAdapter(0x27, LCD_COLS, LCD_ROWS);
    LcdMenu menu(lcdAdapter);
    ```
4.  **Configure Navigation:**

    ```cpp
    SimpleNavConfig navConfig = {
        .menu = &menu,
        .up = 'w',
        .down = 's',
        .enter = ' ',
        .back = 'b',
        .left = 'a',
        .right = 'd',
    };
    ```
5.  **Setup and Loop Functions:**

    ```cpp
    void setup() {
        Serial.begin(9600);
        menu.initialize(mainMenu);
    }

    void loop() {
        if (!Serial.available()) return;
        char command = Serial.read();
        processWithSimpleCommand(&navConfig, command);
    }
    ```

### Character Set Input

#### Setup

1.  **Include Necessary Headers:**

    ```cpp
    #include <ItemInput.h>
    #include <LcdMenu.h>
    #include <interface/LiquidCrystalI2CAdapter.h>
    #include <utils/SimpleNavConfig.h>
    ```
2.  **Define Menu Items with Input:**

    ```cpp
    MAIN_MENU(
        ITEM_INPUT("Con", inputCallback),
        ITEM_BASIC("Connect to WiFi"),
        ITEM_BASIC("Blink SOS"),
        ITEM_BASIC("Blink random"));
    ```
3.  **Create Character Set:**

    ```cpp
    char* charset = "0123456789"
    ```
4.  **Configure Navigation with Charset:**

    ```cpp
    SimpleNavConfig navConfig = {
        .menu = &menu,
        .up = 'w',
        .down = 's',
        .enter = ' ',
        .back = 'b',
        .left = 'a',
        .right = 'd',
        .clear = 'c',
        .backspace = 'v',
        .charset = charset,
        .charsetSize = 10,
        .charsetPosition = -1,
    };
    ```
5.  **Define Callback Function:**

    ```cpp
    void inputCallback(char *value) {
        Serial.print(F("# "));
        Serial.println(value);
    }
    ```
6.  **Setup and Loop Functions:**

    ```cpp
    void setup() {
        Serial.begin(9600);
        menu.initialize(mainMenu);
    }

    void loop() {
        if (!Serial.available()) return;
        char command = Serial.read();
        processWithSimpleCommand(&navConfig, command);
    }
    ```

### Summary

`SimpleNavConfig` simplifies the process of navigating and controlling `LcdMenu` by allowing developers to define custom key bindings. It supports both basic menu operations and character set input, making it versatile for various applications.

### Examples

{% tabs %}
{% tab title="Basic" %}
{% @github-files/github-code-block url="https://github.com/forntoh/LcdMenu/blob/master/examples/Basic/Basic.ino" %}
{% endtab %}

{% tab title="CharsetInput" %}
{% @github-files/github-code-block url="https://github.com/forntoh/LcdMenu/blob/master/examples/CharsetInput/CharsetInput.ino" %}
{% endtab %}
{% endtabs %}
