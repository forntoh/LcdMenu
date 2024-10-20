// Libraries
#if defined(ARDUINO_ARCH_AVR)
#include <Arduino_FreeRTOS.h>
#endif
#include <ItemSubMenu.h>
#include <ItemToggle.h>
#include <LcdMenu.h>
#include <MenuScreen.h>
#include <display/LiquidCrystal_I2CAdapter.h>
#include <input/KeyboardAdapter.h>
#include <renderer/CharacterDisplayRenderer.h>
#include <utils/printf.h>

// 2x20 LCD Display
#define LCD_ROWS 2
#define LCD_COLS 16

float temperature1;
float temperature2;
float temperature3;

bool relay1State, relay2State, relay3State;

// Relay 1 Toggle
void toggleRelay1(bool isOn) {
    printLog(F("relay1State"));
}

// Relay 2 Toggle
void toggleRelay2(bool isOn) {
    printLog(F("relay2State"));
}

// Relay 3 Toggle
void toggleRelay3(bool isOn) {
    printLog(F("relay3State"));
}

LiquidCrystal_I2C lcd(0x27, LCD_COLS, LCD_ROWS);
LiquidCrystal_I2CAdapter lcdAdapter(&lcd);
CharacterDisplayRenderer renderer(&lcdAdapter, LCD_COLS, LCD_ROWS);
LcdMenu menu(renderer);
KeyboardAdapter keyboard(&menu, &Serial);

extern MenuScreen* relayScreen;
extern MenuScreen* tempScreen;

// clang-format off
MENU_SCREEN(mainScreen, mainItems,
    ITEM_BASIC("Relay Test"),
    ITEM_SUBMENU("Temp. Observing", tempScreen),
    ITEM_BASIC("Hello World"),
    ITEM_BASIC("I'm World"));
// clang-format on

// Sub Menu 1: Relays
// clang-format off
MENU_SCREEN(relayScreen, relayItems, 
    ITEM_TOGGLE("Relay 1", toggleRelay1), 
    ITEM_TOGGLE("Relay 2", toggleRelay2), 
    ITEM_TOGGLE("Relay 3", toggleRelay3));
// clang-format on

// Sub Menu 2: Temperature Values
// clang-format off
MENU_SCREEN(tempScreen, tempItems, 
    ITEM_BASIC(""), 
    ITEM_BASIC(""));
// clang-format on

// RTOS func. to measure temperature value
static void tempMeas(void* pvParameters) {
    (void)pvParameters;
    for (;;) {
        //-------------------BEGIN: TEST WITH RANDOM------------------//
        temperature1 = random(1, 1000) / 100.0;  // Generate random float
        temperature2 = random(1, 2000) / 100.0;  // Generate random float
        //-------------------END: TEST WITH RANDOM--------------------//

        char buffer1[8];
        snprintf(buffer1, sizeof(buffer1), "%.02f mA", temperature1);
        tempItems[0]->setText(buffer1);

        vTaskDelay(500 / portTICK_PERIOD_MS);  // wait for 200ms

        char buffer2[8];
        snprintf(buffer2, sizeof(buffer2), "%.02f V", temperature2);
        tempItems[1]->setText(buffer2);

        if (menu.getScreen() == tempScreen) {
            menu.refresh();
        }
        vTaskDelay(3000 / portTICK_PERIOD_MS);  // wait for three seconds
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
}
