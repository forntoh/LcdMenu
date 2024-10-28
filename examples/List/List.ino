#include <ItemWidget.h>
#include <LcdMenu.h>
#include <MenuScreen.h>
#include <display/LiquidCrystal_I2CAdapter.h>
#include <input/KeyboardAdapter.h>
#include <renderer/CharacterDisplayRenderer.h>
#include <widget/WidgetList.h>

#define LCD_ROWS 2
#define LCD_COLS 16

const char* colors[] = {"Red", "Green", "Blue", "Orange", "Aqua", "Yellow", "Purple", "Pink"};
const char* nums[] = {"5", "7", "9", "12", "32"};

const uint8_t NUMS_COUNT = sizeof(nums) / sizeof(nums[0]);
const uint8_t COLORS_COUNT = sizeof(colors) / sizeof(colors[0]);

// Initialize the main menu items
// clang-format off
MENU_SCREEN(mainScreen, mainItems,
    ITEM_BASIC("List demo"),
    ITEM_WIDGET(
        "Color",
        [](const char* color) { Serial.println(color); },
        WIDGET_LIST(colors, COLORS_COUNT, 0, "%s")),
    ITEM_WIDGET(
        "Num",
        [](const char* num) { Serial.println(num); },
        WIDGET_LIST(nums, NUMS_COUNT, 0, "%s")),
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