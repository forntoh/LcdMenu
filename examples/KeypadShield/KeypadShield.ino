#include <ItemWidget.h>
#include <LcdMenu.h>
#include <MenuScreen.h>
#include <display/LiquidCrystalAdapter.h>
#include <input/AnalogButtonAdapter.h>
#include <renderer/CharacterDisplayRenderer.h>
#include <widget/WidgetList.h>

#define LCD_ROWS 2
#define LCD_COLS 16

static const char* colors[] = { "Red", "Green", "Blue", "Orange", "Aqua", "Yellow", "Purple", "Pink" };
static const uint8_t nums[] = { 5, 7, 9, 12, 32 };

static const uint8_t NUMS_COUNT = sizeof(nums) / sizeof(nums[0]);
static const uint8_t COLORS_COUNT = sizeof(colors) / sizeof(colors[0]);

// Initialize the main menu items
// clang-format off
MENU_SCREEN(mainScreen, mainItems,
    ITEM_BASIC("List demo"),
    ITEM_WIDGET(
        "Color",
        [](const char* color) { Serial.println(color); },
        WIDGET_LIST(colors, COLORS_COUNT, 0, "%s", 0, true)),
    ITEM_WIDGET(
        "Num",
        [](const uint8_t num) { Serial.println(num); },
        WIDGET_LIST(nums, NUMS_COUNT, 0, "%d", 0, true)),
    ITEM_BASIC("Example"));
// clang-format on

// Construct the LcdMenu
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
CharacterDisplayRenderer renderer(new LiquidCrystalAdapter(&lcd, LCD_COLS, LCD_ROWS), LCD_COLS, LCD_ROWS);
LcdMenu menu(renderer);
AnalogButtonAdapter Abutton(&menu, A0);

void setup() {
  Serial.begin(9600);
  renderer.begin();
  menu.setScreen(mainScreen);
}

void loop() {
  Abutton.observe();
}
