#include <LcdMenu.h>
#include <MenuScreen.h>
#include <U8g2lib.h>
#include <display/U8g2DisplayAdapter.h>
#include <input/KeyboardAdapter.h>
#include <renderer/GraphicalDisplayRenderer.h>

// clang-format off
MENU_SCREEN(mainScreen, mainItems,
    ITEM_BASIC("Start service"),
    ITEM_BASIC("Connect to WiFi"),
    ITEM_BASIC("Settings"),
    ITEM_BASIC("Blink SOS"),
    ITEM_BASIC("Blink random"));
// clang-format on

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);
U8g2DisplayAdapter lcdAdapter(&u8g2);
GraphicalDisplayRenderer renderer(&lcdAdapter, 128, 64, 6, 8);
LcdMenu menu(renderer);
KeyboardAdapter keyboard(&menu, &Serial);

void setup() {
    Serial.begin(9600);
    renderer.begin();
    menu.setScreen(mainScreen);
}

void loop() { keyboard.observe(); }
