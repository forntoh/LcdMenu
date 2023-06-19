---
description: This example demonstrates how to enable timeout using the LcdMenu library
---

# Timeout

After the specified time, the LCD display will turn off and will turn back on after an action is performed.

The default timeout is 10 seconds.

### Configuration

In the setup function, use the following function to configure the timeout duration.

<pre class="language-cpp"><code class="lang-cpp">menu.setupLcdWithMenu(0x27, mainMenu, <a data-footnote-ref href="#user-content-fn-1">20000</a>)
</code></pre>

### Usage

{% hint style="warning" %}
In the `loop` ensure that you have the following in the first line
{% endhint %}

```cpp
menu.updateTimer();
```

## Complete Code

Go to [.../examples/MenuTimeout/MenuTimeout.ino](https://github.com/forntoh/LcdMenu/tree/master/examples/MenuTimeout/MenuTimeout.ino)

[^1]: Timeout duration in milliseconds
