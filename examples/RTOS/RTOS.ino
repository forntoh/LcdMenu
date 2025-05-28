// Libraries
#if defined(ARDUINO_ARCH_AVR)
#include <Arduino_FreeRTOS.h>
#endif
#include <ItemSubMenu.h>
#include <ItemToggle.h>
#include <ItemValue.h>
#include <LcdMenu.h>
#include <MenuScreen.h>
#include <display/LiquidCrystal_I2CAdapter.h>
#include <input/KeyboardAdapter.h>
#include <renderer/CharacterDisplayRenderer.h>
#include <utils/printf.h>

// 2x20 LCD Display
#define LCD_ROWS 2
#define LCD_COLS 16

uint8_t temperature1, temperature2, temperature3;
bool relay1State, relay2State, relay3State;

LiquidCrystal_I2C lcd(0x27, LCD_COLS, LCD_ROWS);
LiquidCrystal_I2CAdapter lcdAdapter(&lcd);
CharacterDisplayRenderer renderer(&lcdAdapter, LCD_COLS, LCD_ROWS);
LcdMenu menu(renderer);
KeyboardAdapter keyboard(&menu, &Serial);

extern MenuScreen* relayScreen;
extern MenuScreen* tempScreen;

// clang-format off
MENU_SCREEN(mainScreen, mainItems,
    ITEM_SUBMENU("Relay Test", relayScreen),
    ITEM_SUBMENU("Temp. Observing", tempScreen),
    ITEM_BASIC("Hello World"),
    ITEM_BASIC("I'm World"));
// clang-format on

// Sub Menu 1: Relays
// clang-format off
MENU_SCREEN(relayScreen, relayItems, 
    ITEM_TOGGLE("Relay 1", [&](bool isOn) { relay1State = isOn; }), 
    ITEM_TOGGLE("Relay 2", [&](bool isOn) { relay2State = isOn; }), 
    ITEM_TOGGLE("Relay 3", [&](bool isOn) { relay3State = isOn; }));
// clang-format on

// Sub Menu 2: Temperature Values
// clang-format off
MENU_SCREEN(tempScreen, tempItems, 
    ITEM_VALUE("Temp1", temperature1, "%d C"), 
    ITEM_VALUE("Temp2", temperature2, "%d C"),
    ITEM_VALUE("Temp3", temperature3, "%d C"));
// clang-format on

// RTOS func. to measure temperature value
static void tempMeas(void* pvParameters) {
    (void)pvParameters;
    for (;;) {
        //-------------------BEGIN: TEST WITH RANDOM------------------//
        // Update temperature1 only if relay1 is on
        if (relay1State) temperature1 = random(1, 101);
        // Update temperature2 only if relay2 is on
        if (relay2State) temperature2 = random(1, 101);
        // Update temperature3 only if relay3 is on
        if (relay3State) temperature3 = random(1, 101);
        //-------------------END: TEST WITH RANDOM--------------------//
        vTaskDelay(2000 / portTICK_PERIOD_MS);  // wait for two seconds
    }
}

void setup() {
    Serial.begin(9600);
    // LCD activation
    renderer.begin();
    menu.setScreen(mainScreen);
    // Run RTOS func.
    xTaskCreate(tempMeas, "tempMeas", 128, NULL, tskIDLE_PRIORITY + 1, NULL);
    // Setup random seed
    pinMode(A0, INPUT);
    randomSeed(analogRead(A0));
}

void loop() {
    renderer.updateTimer();
    keyboard.observe();
    menu.poll();
}
