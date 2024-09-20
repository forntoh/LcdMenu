#include <ItemToggle.h>
#include <LcdMenu.h>
#include <MenuScreen.h>
#include <display/LiquidCrystal_I2CAdapter.h>
#include <input/DigitalInputAdapter.h>

#define LCD_ROWS 2
#define LCD_COLS 16

// Declare the call back function
void toggleBacklight(uint16_t isOn);

// clang-format off
MENU_SCREEN(mainScreen, mainItems,
    ITEM_BASIC("Start service"),
    ITEM_BASIC("Connect to WiFi"),
    ITEM_TOGGLE("Backlight", toggleBacklight),
    ITEM_BASIC("Blink SOS"),
    ITEM_BASIC("Blink random"));
// clang-format on

LiquidCrystal_I2C lcd(0x27, LCD_COLS, LCD_ROWS);
LiquidCrystal_I2CAdapter lcdAdapter(&lcd, LCD_COLS, LCD_ROWS);
LcdMenu menu(lcdAdapter);
DigitalInputAdapter upBtn(&menu, 2, UP);
DigitalInputAdapter downBtn(&menu, 3, DOWN);
DigitalInputAdapter enterBtn(&menu, 4, ENTER);

void setup() {
    Serial.begin(9600);
    lcdAdapter.begin();
    menu.setScreen(mainScreen);
}

void loop() {
    upBtn.observe();
    downBtn.observe();
    enterBtn.observe();
}
/**
 * Define callback
 */
void toggleBacklight(uint16_t isOn) { lcdAdapter.setBacklight(isOn); }
