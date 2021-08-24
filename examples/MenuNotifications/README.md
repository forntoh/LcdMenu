# Example 05 Menu Notifications

This example demonstrates how to display Notifications using the LcdMenu library.

After the specified time, the notification message disappears.

In this example When the **#** key is pressed, the notification displays for two (2) seconds.

![Notice](https://i.imgur.com/aXuQohA.gif)

## Requirements

- Keyboard
- LcdDisplay

## Usage

Show notification

```cpp
menu.displayNotification("Success", 2000);
```

In the `loop` ensure that you have this in the first line

```cpp
menu.updateTimer();
```

## Function reference

```cpp
/**
 * Show a message at the bottom of the screen
 *
 * @param message   message to display
 * @param duration  how long to display the message
 */
void displayNotification(char* message, unsigned int duration);
```

```cpp
/**
 * Executes any delayed task when appropriate time reaches
 */
void updateTimer();
```

## Complete Code

Go to [.../examples/MenuNotifications/MenuNotifications.ino](https://github.com/forntoh/LcdMenu/tree/master/examples/MenuNotifications/MenuNotifications.ino)
