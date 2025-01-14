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

//Center value for each button
#define btn_val_RIGHT 20	// BACKSPACE
#define btn_val_UP 100		// UP
#define btn_val_DOWN 250	// DOWN
#define btn_val_LEFT 400	// BACK
#define btn_val_SELECT 640	// ENTER

#define btn_MARGIN 20		// Margin of defined value
#define btn_val_DEFAULT 1000	// Value when no button is pressed
#define btn_pressTime 300	// Last time the button was pressed

class AnalogButtonAdapter : public InputInterface {
  private:
    uint8_t pinNumber;
    unsigned long lastPressTime = 0;  // Last time the button was pressed
    
  public:
    AnalogButtonAdapter(LcdMenu* menu, uint8_t pinNumber)
        : InputInterface(menu), pinNumber(pinNumber) {
    }

    void observe() override {

        int16_t ar = analogRead(pinNumber);	//Read value from pin
        unsigned long currentTime = millis();
        if (ar < btn_val_DEFAULT) {     	//Check if the button is pressed
        	if(currentTime - lastPressTime > btn_pressTime){	//Check if 
        	        lastPressTime = currentTime;
        		if (ar < (btn_val_RIGHT + btn_MARGIN) && ar > (btn_val_RIGHT-btn_MARGIN)) menu->process(BACKSPACE); 
        		else if (ar < (btn_val_UP + btn_MARGIN) && ar > (btn_val_UP-btn_MARGIN))  menu->process(UP);
        		else if (ar < (btn_val_DOWN + btn_MARGIN) && ar > (btn_val_DOWN-btn_MARGIN))  menu->process(DOWN);
        		else if (ar < (btn_val_LEFT + btn_MARGIN) && ar > (btn_val_LEFT-btn_MARGIN)) menu->process(BACK);
        		else if (ar < (btn_val_SELECT + btn_MARGIN) && ar > (btn_val_SELECT-btn_MARGIN))  menu->process(ENTER);  
        	}
    	}
    }
};
