#include <ItemBack.h>
#include <ItemSubMenu.h>
#include <LcdMenu.h>
#include <MenuScreen.h>
#include <display/LiquidCrystal_I2CAdapter.h>
#include <input/KeyboardAdapter.h>
#include <renderer/CharacterDisplayRenderer.h>

#define LCD_ROWS 2
#define LCD_COLS 16

extern MenuScreen* settingsScreen;

// Define the main menu
// clang-format off
MENU_SCREEN(mainScreen, mainItems,
    ITEM_BASIC("Start service"),
    ITEM_BASIC("Connect to WiFi"),
    ITEM_SUBMENU("Settings", settingsScreen),
    ITEM_BASIC("Blink SOS"),
    ITEM_BASIC("Blink random"));

// Create submenu and precise its parent
MENU_SCREEN(settingsScreen, settingsItems,
    ITEM_BASIC("Backlight"),
    ITEM_BASIC("Contrast"),
    ITEM_BACK());
// clang-format on

LiquidCrystal_I2C lcd(0x27, LCD_COLS, LCD_ROWS);
CharacterDisplayRenderer renderer(new LiquidCrystal_I2CAdapter(&lcd), LCD_COLS, LCD_ROWS);
LcdMenu menu(renderer);

// Initialize the KeyboardAdapter
KeyboardAdapter keyboard(&menu, &Serial);

void setup() {
    Serial.begin(9600);
    renderer.begin();
    menu.setScreen(mainScreen);
}

void loop() {
    keyboard.observe();
}