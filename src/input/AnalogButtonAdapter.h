#pragma once

#include "./InputInterface.h"
/**
 * @class AnalogButtonAdapter
 * @brief Adapter class to handle analog button array as input for an LCD menu.
 *
 * Wiring example
 * https://www.instructables.com/How-to-Multiple-Buttons-on-1-Analog-Pin-Arduino-Tu/
 *
 * This class implements the InputInterface to provide button input handling
 * for an LCD menu. It observes analog button presses and processes commands
 * accordingly. Made for LCD 1602 LCD Keypad Shield
 *
 * @details
 * It initializes the button and observes its state to trigger menu commands
 * when the button is pressed.
 *
 * @param menu Pointer to the LcdMenu object that this adapter will interact
 * with.
 * @param pinNumber is analog pin where is connected the array of buttons
 * @param triggerValue Value of input pin, when button press is registered
 * @param margin How much input value can change to be still valid button press
 * @param command The command byte that will be sent to the menu when the button
 * is pressed.
 */

class ButtonConfig {
  public:
    static constexpr int16_t MAX_VALUE = 1023;  // (0-1023 for 10-bit ADC)
    static constexpr unsigned long PRESS_TIME_MS = 300;
    static constexpr uint16_t DEFAULT_MARGIN = 20;
};

struct RepeatState {
    unsigned long delay;
    unsigned long interval;
    unsigned long pressStart = 0;
    unsigned long lastRepeat = 0;

    RepeatState(unsigned long d = 0, unsigned long i = 0)
        : delay(d), interval(i) {}

    bool enabled() const { return delay && interval; }

    void reset() {
        pressStart = 0;
        lastRepeat = 0;
    }

    void start(unsigned long now) {
        pressStart = now;
        lastRepeat = 0;
    }

    bool shouldRepeat(unsigned long now) {
        if (!enabled()) return false;
        if (now - pressStart >= delay &&
            (lastRepeat == 0 || now - lastRepeat >= interval)) {
            lastRepeat = now;
            return true;
        }
        return false;
    }
};

class AnalogButtonAdapter : public InputInterface {
  private:
    uint8_t pinNumber;
    uint16_t triggerValue;
    uint16_t margin;
    byte command;
    unsigned long lastPressTime = 0;  // Last time the button was pressed
    RepeatState repeat;
    unsigned long debounceTime;
    bool wasPressed = false;

  public:
    AnalogButtonAdapter(
        LcdMenu* menu,
        uint8_t pinNumber,
        uint16_t triggerValue,
        uint16_t margin,
        byte command,
        unsigned long repeatDelay = 0,
        unsigned long repeatInterval = 0,
        unsigned long debounceTime = ButtonConfig::PRESS_TIME_MS)
        : InputInterface(menu), margin(ButtonConfig::DEFAULT_MARGIN), command(command), repeat(repeatDelay, repeatInterval),
          debounceTime(debounceTime) {}

    void observe() override {
        int16_t analogValue = analogRead(pinNumber);
        if (analogValue >= ButtonConfig::MAX_VALUE) {
            wasPressed = false;
            repeat.reset();
            return;
        }

        bool pressed = analogValue <= (triggerValue + margin) && analogValue >= (triggerValue - margin);
        unsigned long currentTime = millis();

        if (!pressed) {
            wasPressed = false;
            repeat.reset();
            return;
        }

        if (!wasPressed) {
            if (currentTime - lastPressTime <= debounceTime) {
                return;
            }
            lastPressTime = currentTime;
            wasPressed = true;
            menu->process(command);
            repeat.start(currentTime);
        } else if (repeat.shouldRepeat(currentTime)) {
            menu->process(command);
        }
    }
};
