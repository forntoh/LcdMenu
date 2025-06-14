#pragma once

#include <Arduino.h>
#include <SimpleRotary.h>
#include <LcdMenu.h>
#include <MenuScreen.h>
#include <ItemValue.h>
#include <ItemCommand.h>
#include <ItemSubMenu.h>
#include <ItemInputCharset.h>

// IMPORTANT: We are *not* inheriting from SimpleRotaryAdapter. Instead, we
// re-implement its exact logic plus our extra “timing” hook.

#ifndef LONG_PRESS_DURATION
  #define LONG_PRESS_DURATION 1000
#endif

#ifndef DOUBLE_PRESS_THRESHOLD
  #define DOUBLE_PRESS_THRESHOLD 300
#endif

/**
 * @class TimedRotaryAdapter
 * @brief A drop-in replacement for SimpleRotaryAdapter that also updates
 *        a user-supplied “lastActivityTime” whenever the encoder is used.
 *
 * How to use:
 *   - In your main sketch, declare:
 *       unsigned long lastActivityTime = 0;
 *       SimpleRotary encoder( pinA, pinB, pinButton );
 *       TimedRotaryAdapter rotaryInput(&menu, &encoder, &lastActivityTime);
 *
 *   - In setup():
 *       lastActivityTime = millis();
 *       // (Optionally) menu.setInput(&rotaryInput);
 *
 *   - In loop():
 *       rotaryInput.observe();
 *       menu.poll();
 *       // … check (millis() - lastActivityTime) to turn backlight on/off …
 */
class TimedRotaryAdapter {
  private:
    LcdMenu*     menu;               ///< The menu to control
    SimpleRotary* encoder;           ///< The encoder to read
    unsigned long* pLastActivity;    ///< Points to your global inactivity timer

    // State for detecting double-press vs. short-press vs. long-press:
    unsigned long lastPressTime = 0;
    bool pendingEnter = false;

  public:
    /**
     * @param menuPtr    Pointer to your LcdMenu instance
     * @param encPtr     Pointer to a SimpleRotary object
     * @param lastActPtr Pointer to your “unsigned long lastActivityTime” variable
     */
    TimedRotaryAdapter(LcdMenu* menuPtr, SimpleRotary* encPtr, unsigned long* lastActPtr)
      : menu(menuPtr), encoder(encPtr), pLastActivity(lastActPtr)
    {
      // Initialize the “lastActivityTime” so it doesn't immediately time out:
      *pLastActivity = millis();
    }

    /**
     * Call this at the top of loop() every time you want to poll the encoder.
     * It replicates SimpleRotaryAdapter::observe() logic, but also bumps
     * *pLastActivity = millis() whenever rotation or any button action occurs.
     */
    void observe() {
      // 1) Read rotation from encoder:
      uint8_t rotation = encoder->rotate();
      if (rotation == 1) {
        menu->process(DOWN);
      }
      else if (rotation == 2) {
        menu->process(UP);
      }

      // 2) Read button press type: 0 = none, 1 = (short or pending short), 2 = long
      uint8_t pressType = encoder->pushType(LONG_PRESS_DURATION);
      unsigned long now = millis();

      if (pressType == 1) {
        // A “press” has happened (either short-press pending or start of long-press)
        if (pendingEnter) {
          // If already pending, and timestamp < DOUBLE_PRESS_THRESHOLD, double-press
          if (now - lastPressTime < DOUBLE_PRESS_THRESHOLD) {
            menu->process(BACKSPACE);
            pendingEnter = false;
          }
        }
        else {
          // Start a pending “enter” attempt
          pendingEnter = true;
          lastPressTime = now;
        }
      }
      else if (pressType == 2) {
        // Long press
        menu->process(BACK);
        pendingEnter = false;
      }

      // 3) If pendingEnter has timed out (i.e. > DOUBLE_PRESS_THRESHOLD without a 2nd click),
      //    treat it as a real “ENTER” (short press). Also ensure we're not in “edit mode.”
      if (pendingEnter) {
        // Two cases:
        //   a) If not in edit mode, and we are pending → ENTER immediately.
        //   b) Or if enough time has passed to exceed DOUBLE_PRESS_THRESHOLD → ENTER.
        bool notEditing = !menu->getRenderer()->isInEditMode();
        if ( (notEditing && pendingEnter) || (now - lastPressTime >= DOUBLE_PRESS_THRESHOLD) ) {
          menu->process(ENTER);
          pendingEnter = false;
        }
      }

      // 4) **Timing Hook**: if there was ANY rotation or ANY button action, update lastActivityTime
      if (rotation != 0 || pressType != 0) {
        *pLastActivity = now;
      }
    }
};
