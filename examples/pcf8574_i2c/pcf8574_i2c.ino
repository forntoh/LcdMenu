#include <LcdMenu.h>
#include <MenuScreen.h>
#include <display/LCD_I2CAdapter.h>
#include <input/KeyboardAdapter.h>
#include <renderer/CharacterDisplayRenderer.h>
//
// EXAMPLE FOR blackhack\LCD_I2C Library
// PCF8574T I2C Screen
//
//
#define I2C_ADDR 0x27 // I2C Address for the PCF8574T
#define LCD_COLS 16
#define LCD_ROWS 2

// clang-format off
MENU_SCREEN(mainScreen, mainItems,
    ITEM_BASIC("Start service"),
    ITEM_BASIC("Connect to WiFi"),
    ITEM_BASIC("Settings"),
    ITEM_BASIC("Blink SOS"),
    ITEM_BASIC("Blink random"));
// clang-format on

LCD_I2C lcd(I2C_ADDR, LCD_COLS, LCD_ROWS);
LCD_I2CAdapter lcdAdapter(&lcd);
CharacterDisplayRenderer renderer(&lcdAdapter, LCD_COLS, LCD_ROWS);
LcdMenu menu(renderer);
KeyboardAdapter keyboard(&menu, &Serial);

void setup() {
    Serial.begin(9600);
    lcd.begin();
    renderer.begin();
    menu.setScreen(mainScreen);
}

void loop() {
    keyboard.observe();
}
