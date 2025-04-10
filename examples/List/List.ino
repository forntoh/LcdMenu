#include <ItemList.h>
#include <LcdMenu.h>
#include <MenuScreen.h>
#include <display/LiquidCrystal_I2CAdapter.h>
#include <input/KeyboardAdapter.h>
#include <renderer/CharacterDisplayRenderer.h>

#define LCD_ROWS 2
#define LCD_COLS 16

std::vector<const char*> colors = {"Red", "Green", "Blue", "Orange", "Aqua", "Yellow", "Purple", "Pink"};
std::vector<uint8_t> nums = {5, 7, 9, 12, 32};

// Initialize the main menu items
// clang-format off
MENU_SCREEN(mainScreen, mainItems,
    ITEM_BASIC("List demo"),
    ITEM_LIST("Color", colors, [](const uint8_t color) { Serial.println(colors[color]); }),
    ITEM_LIST("Num", nums, [](const uint8_t num) { Serial.println(nums[num]); }, 0, "%d", 0, true),
    ITEM_BASIC("Example"));
// clang-format on

// Construct the LcdMenu
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
