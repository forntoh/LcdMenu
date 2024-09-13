// Libraries
#include <Arduino_FreeRTOS.h>
#include <ItemSubMenu.h>
#include <ItemToggle.h>
#include <LcdMenu.h>
#include <interface/LiquidCrystalI2CAdapter.h>
#include <utils/SimpleNavConfig.h>

// 2x20 LCD Display
#define LCD_ROWS 2
#define LCD_COLS 16

float temperature1;
float temperature2;
float temperature3;

bool relay1State, relay2State, relay3State;
// Main Menu Definings
extern MenuItem *relayTest[];
extern MenuItem *tempObs[];

// Relay 1 Toggle
void toggleRelay1(uint16_t isOn) {
    printCmd(F("relay1State"));
}

// Relay 2 Toggle
void toggleRelay2(uint16_t isOn) {
    printCmd(F("relay2State"));
}

// Relay 3 Toggle
void toggleRelay3(uint16_t isOn) {
    printCmd(F("relay3State"));
}

LiquidCrystalI2CAdapter lcdAdapter(0x27, LCD_COLS, LCD_ROWS);
LcdMenu menu(lcdAdapter);

SimpleNavConfig navConfig = {
    .menu = &menu,
    .up = 'w',
    .down = 's',
    .enter = ' ',
    .back = 'b',
    .left = 'a',
    .right = 'd',
};

MAIN_MENU(
    ITEM_BASIC("Relay Test"),
    ITEM_SUBMENU("Temp. Observing", tempObs),
    ITEM_BASIC("Hello World"),
    ITEM_BASIC("I'm World"));

// Sub Menu 1: Relays
SUB_MENU(relayTest, mainMenu, ITEM_TOGGLE("Relay 1", toggleRelay1), ITEM_TOGGLE("Relay 2", toggleRelay2), ITEM_TOGGLE("Relay 3", toggleRelay3));

// Sub Menu 2: Temperature Values
SUB_MENU(tempObs, mainMenu, ITEM_BASIC(""), ITEM_BASIC(""), ITEM_BASIC(""));

// RTOS func. to measure temperature value
static void tempMeas(void *pvParameters) {
    (void)pvParameters;
    for (;;) {
        //-------------------BEGIN: TEST WITH RANDOM------------------//
        temperature1 = random(1, 1000) / 100.0; // Generate random float
        temperature2 = random(1, 2000) / 100.0; // Generate random float
        temperature3 = random(1, 3000) / 100.0; // Generate random float
        //-------------------END: TEST WITH RANDOM--------------------//

        char buffer1[8];
        tempObs[1]->setText(dtostrf(temperature1, 5, 2, buffer1));

        char buffer2[8];
        tempObs[2]->setText(dtostrf(temperature2, 5, 2, buffer2));

        char buffer3[8];
        tempObs[3]->setText(dtostrf(temperature3, 5, 2, buffer3));

        menu.update();
        vTaskDelay(5000 / portTICK_PERIOD_MS); // wait for five seconds
    }
}

// RTOS func to read serial input from keyboard
static void keyPad(void *pvParameters) {
    (void)pvParameters; // Unused parameter

    for (;;) {
        char command = Serial.read();
        processWithSimpleCommand(&navConfig, command);
        vTaskDelay(1);
    }
}

void setup() {
    Serial.begin(9600);
    // LCD activation
    menu.initialize(mainMenu);
    // Run RTOS func.
    xTaskCreate(keyPad, "keyPad", 128, NULL, tskIDLE_PRIORITY + 2, NULL);
    xTaskCreate(tempMeas, "tempMeas", 128, NULL, tskIDLE_PRIORITY + 1, NULL);
    // Setup random seed
    pinMode(A0, INPUT);
    randomSeed(analogRead(A0));
}

void loop() {
    lcdAdapter.updateTimer();
}