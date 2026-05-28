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

U8G2_ST7920_128X64_F_HW_SPI u8g2(U8G2_R0, 10, U8X8_PIN_NONE);
U8g2DisplayAdapter display(&u8g2);
GraphicalDisplayRenderer renderer(&display, u8g2_font_6x10_tf);
LcdMenu menu(renderer);
KeyboardAdapter keyboard(&menu, &Serial);

void setup() {
    Serial.begin(9600);
    renderer.begin();
    renderer.setItemFont(mainItems[0], u8g2_font_7x13B_tf);
    menu.setScreen(mainScreen);
}

void loop() {
    keyboard.observe();
}
