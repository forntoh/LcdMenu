#include <ItemSubMenu.h>
#include <LcdMenu.h>
#include <MenuScreen.h>
#include <display/LiquidCrystal_I2CAdapter.h>
#include <input/KeyboardAdapter.h>
#include <renderer/CharacterDisplayRenderer.h>

#define LCD_ROWS 2
#define LCD_COLS 16

extern MenuScreen* settingsScreen;
extern MenuScreen* settings2Screen;

// Define the main menu
// clang-format off
MENU_SCREEN(mainScreen, mainItems,
    ITEM_SUBMENU("Settings", settingsScreen),
    ITEM_BASIC("Start service"),
    ITEM_BASIC("Connect to WiFi"),
    ITEM_BASIC("Blink SOS"),
    ITEM_BASIC("Blink random"),
    ITEM_SUBMENU("Settings 2", settings2Screen));

// Create submenu and precise its parent
MENU_SCREEN(settingsScreen, settingsItems,
    ITEM_BASIC("Backlight"),
    ITEM_BASIC("Contrast"),
    ITEM_BASIC("Contrast1"),
    ITEM_BASIC("Contrast2"),
    ITEM_BASIC("Contrast3"),
    ITEM_BASIC("Contrast4"),
    ITEM_BASIC("Contrast5"),
    ITEM_BASIC("Contrast6"),
    ITEM_BASIC("Contrast7"),
    ITEM_SUBMENU("Settings2", settings2Screen));

MENU_SCREEN(settings2Screen, settings2Items,
    ITEM_BASIC("Backlight"),
    ITEM_BASIC("Contrast"));
// clang-format on

LiquidCrystal_I2C lcd(0x27, LCD_COLS, LCD_ROWS);
CharacterDisplayRenderer renderer(new LiquidCrystal_I2CAdapter(&lcd), LCD_COLS, LCD_ROWS);
LcdMenu menu(renderer);
KeyboardAdapter keyboard(&menu, &Serial);

void setup() {
    Serial.begin(9600);
    renderer.begin();
    menu.setScreen(mainScreen);
}

void loop() {
    keyboard.observe();
}
