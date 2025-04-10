#include <ItemList.h>
#include <LcdMenu.h>
#include <MenuScreen.h>
#include <display/LiquidCrystalAdapter.h>
#include <input/AnalogButtonAdapter.h>
#include <renderer/CharacterDisplayRenderer.h>

#define LCD_ROWS 2
#define LCD_COLS 16

std::vector<const char*> colors = {"Red", "Green", "Blue", "Orange", "Aqua", "Yellow", "Purple", "Pink"};
std::vector<uint8_t> nums = {5, 7, 9, 12, 32};

// Initialize the main menu items
// clang-format off
MENU_SCREEN(mainScreen, mainItems,
    ITEM_BASIC("Keypad demo"),
    ITEM_LIST("Color", colors, [](const uint8_t color) { Serial.println(colors[color]); }),
    ITEM_LIST("Num", nums, [](const uint8_t num) { Serial.println(nums[num]); }, 0, "%d", 0, true),
    ITEM_BASIC("Example"));
// clang-format on

// Construct the LcdMenu
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
CharacterDisplayRenderer renderer(new LiquidCrystalAdapter(&lcd, LCD_COLS, LCD_ROWS), LCD_COLS, LCD_ROWS);
LcdMenu menu(renderer);

AnalogButtonAdapter ButtonBackSpace(&menu, A0, 20, BACKSPACE);
AnalogButtonAdapter ButtonUp(&menu, A0, 100, 20, UP);
AnalogButtonAdapter ButtonDown(&menu, A0, 250, 20, DOWN);
AnalogButtonAdapter ButtonBack(&menu, A0, 400, 20, BACK);
AnalogButtonAdapter ButtonEnter(&menu, A0, 640, 20, ENTER);

void setup() {
    Serial.begin(9600);
    renderer.begin();
    menu.setScreen(mainScreen);
}

void loop() {
    ButtonBackSpace.observe();
    ButtonUp.observe();
    ButtonDown.observe();
    ButtonBack.observe();
    ButtonEnter.observe();
}
