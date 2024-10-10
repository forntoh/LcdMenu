#include <LcdMenu.h>
#include <MenuScreen.h>
#include <display/SSD1803A_I2CAdapter.h>
#include <input/KeyboardAdapter.h>
#include <renderer/CharacterDisplayRenderer.h>

// clang-format off
MENU_SCREEN(mainScreen, mainItems,
    ITEM_BASIC("Start service"),
    ITEM_BASIC("Connect to WiFi"),
    ITEM_BASIC("Settings"),
    ITEM_BASIC("Blink SOS"),
    ITEM_BASIC("Blink random"));
// clang-format on

SSD1803A_I2C lcd(0x3D);
SSD1803A_I2CAdapter lcdAdapter(&lcd);
CharacterDisplayRenderer renderer(&lcdAdapter, 20, 4);
LcdMenu menu(renderer);
KeyboardAdapter keyboard(&menu, &Serial);

void setup() {
    Serial.begin(9600);
    lcd.begin(DOGM204);  // initialize DOGM204
    // lcd.begin(WIRE1, DOGS164);  // initialize DOGS164 on WIRE1
    // lcd.begin(WIRE1, DOGS104);  // initialize DOGS104 on WIRE1
    renderer.begin();
    menu.setScreen(mainScreen);
}

void loop() {
    keyboard.observe();
}