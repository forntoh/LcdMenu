---
description: This example demonstrates how to enable timeout using the LcdMenu library
---

# Timeout

After the specified time, the LCD display will turn off and will turn back on after an action is performed.

The default timeout is 10 seconds.

### Configuration

Define the timeout duration (optional)

<pre class="language-arduino"><code class="lang-arduino"><strong>#define DISPLAY_TIMEOUT <a data-footnote-ref href="#user-content-fn-1">20000</a>
</strong></code></pre>

### Usage

{% hint style="warning" %}
In the `loop` ensure that you have the following in the first line

```cpp
lcdAdapter.updateTimer();
```
{% endhint %}

### Complete Code

Go to [.../examples/MenuTimeout/MenuTimeout.ino](https://github.com/forntoh/LcdMenu/tree/master/examples/MenuTimeout/MenuTimeout.ino)

[^1]: Timeout duration in milliseconds
