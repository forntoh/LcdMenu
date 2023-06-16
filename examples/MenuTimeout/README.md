# Example 05 Menu Timeout

This example demonstrates how to enable timeout using the LcdMenu library.

After the specified time, the LcdDisplay will turn off and will turn back on after an action is performed.

## Requirements

- Keyboard
- LcdDisplay

## Usage

In the `loop` ensure that you have this in the first line

```cpp
menu.updateTimer();
```

## Function reference

```cpp
/**
 * Update timer and turn off display on timeout
 */
void updateTimer();
```

## Complete Code

Go to [.../examples/MenuTimeout/MenuTimeout.ino](https://github.com/forntoh/LcdMenu/tree/master/examples/MenuTimeout/MenuTimeout.ino)
