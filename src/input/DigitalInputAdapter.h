#pragma once

#include "InputInterface.h"

/**
 * @class DigitalInputAdapter
 * @brief A class to handle digital input devices.
 *
 * This class implements the InputInterface and provides functionality to read the state of
 * various devices, allowing the user to navigate through a menu system.
 *
 * This class can be used to interface with various digital input devices such as:
 * - Push Buttons
 * - Toggle Switches
 * - Limit Switches
 * - Reed Switches
 * - Infrared Sensors
 * - Hall Effect Sensors
 * - Tilt Sensors
 * - Digital Temperature Sensors
 * - Proximity Sensors
 * - Rotary Encoders
 *
 * For example, you can use this class to read the state of a push button connected to
 * an Arduino and use it to navigate through a menu.
 *
 * It provides debouncing functionality to ensure stable readings from the input device.
 *
 * Example usage:
 * @code
 * DigitalInputAdapter upBtn(&menu, 2, UP); // Pin 2
 * DigitalInputAdapter downBtn(&menu, 3, DOWN); // Pin 3
 * void setup() {
 *     // Initialization code
 * }
 * void loop() {
 *     upBtn.observe();
 *     downBtn.observe();
 *     // Other code
 * }
 * @endcode
 *
 * @param menu Pointer to the LcdMenu instance that this adapter will control.
 * @param pin The digital pin number to which the input device is connected.
 * @param command The command to be sent to the menu when the input device is HIGH.
 * @param debounceDelay The delay in milliseconds for debouncing the input (default is 50ms).
 */
class DigitalInputAdapter : public InputInterface {
  private:
    uint8_t pin;
    byte command;
    uint16_t debounceDelay;
    uint32_t lastDebounceTime = 0;
    bool lastButtonState;
    bool buttonState;

  public:
    DigitalInputAdapter(
        LcdMenu* menu,
        uint8_t pin,
        byte command,
        uint16_t debounceDelay = 50)
        : InputInterface(menu),
          pin(pin),
          command(command),
          debounceDelay(debounceDelay),
          lastButtonState(LOW),
          buttonState(LOW) {
        pinMode(pin, INPUT);
    }

    void observe() override {
        bool reading = digitalRead(pin);
        if (reading != lastButtonState) {
            lastDebounceTime = millis();
        }
        if ((millis() - lastDebounceTime) > debounceDelay) {
            if (reading != buttonState) {
                buttonState = reading;
                if (buttonState == HIGH) {
                    menu->process(command);
                }
            }
        }
        lastButtonState = reading;
    }
};