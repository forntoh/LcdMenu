#pragma once

#include "./InputInterface.h"
/**
 * @class AnalogButtonAdapter
 * @brief Adapter class to handle analog button array as input for an LCD menu.
 *
 * Wiring example https://www.instructables.com/How-to-Multiple-Buttons-on-1-Analog-Pin-Arduino-Tu/
 *
 * This class implements the InputInterface to provide button input handling
 * for an LCD menu. It observes analog button presses and processes commands accordingly.
 * Made for LCD 1602 LCD Keypad Shield
 *
 * @details
 * It initializes the button and observes its state to trigger menu commands when
 * the button is pressed.
 *
 * @param menu Pointer to the LcdMenu object that this adapter will interact with.
 * @param pinNumber is analog pin where is connected the array of buttons
 */

class ButtonConfig {
  public:
    static constexpr int16_t MAX_VALUE = 1023; // (0-1023 for 10-bit ADC)
    static constexpr unsigned long PRESS_TIME_MS = 300;
    static constexpr uint16_t DEFAULT_MARGIN = 20;
};

class AnalogButtonAdapter : public InputInterface {
  private:
    uint8_t pinNumber;
    uint16_t triggerValue;
    uint16_t margin;
    byte command;
    unsigned long lastPressTime = 0;  // Last time the button was pressed

  public:
    AnalogButtonAdapter(LcdMenu* menu, uint8_t pinNumber, uint16_t triggerValue, uint16_t margin, byte command)
        : InputInterface(menu), pinNumber(pinNumber), triggerValue(triggerValue), margin(margin), command(command) {
    }
    AnalogButtonAdapter(LcdMenu* menu, uint8_t pinNumber, uint16_t triggerValue, byte command)
        : InputInterface(menu), pinNumber(pinNumber), triggerValue(triggerValue), margin(ButtonConfig::DEFAULT_MARGIN), command(command) {
    }

    void observe() override {
        // Read analog value from pin
        int16_t analogValue = analogRead(pinNumber);  // Read value from pin

        // Ignore readings above the maximum possible value (no button pressed)
        if (analogValue >= ButtonConfig::MAX_VALUE) {
            return;
        }

        // Apply debouncing
        unsigned long currentTime = millis();
        if (currentTime - lastPressTime <= ButtonConfig::PRESS_TIME_MS) {
            return;
        }

        // Process button press
        lastPressTime = currentTime;

        if (analogValue < (triggerValue + margin) && analogValue > (triggerValue - margin)) {
            menu->process(command);
        }
    }
};
