#include <Button.h>
#include <ItemList.h>
#include <ItemToggle.h>
#include <LcdMenu.h>
#include <MenuScreen.h>
#include <display/LiquidCrystal_I2CAdapter.h>
#include <input/ButtonAdapter.h>
#include <input/JoystickAdapter.h>
#include <renderer/CharacterDisplayRenderer.h>

#define LCD_ROWS 2
#define LCD_COLS 16

// Sample menu
std::vector<const char*> colors = {"Red", "Green", "Blue", "Orange"};

// clang-format off
MENU_SCREEN(mainScreen, mainItems,
    ITEM_LIST("Color", colors, [](const uint8_t color) { Serial.println(colors[color]); }),
    ITEM_TOGGLE("Light", [](bool on){ Serial.println(on); }));
// clang-format on

LiquidCrystal_I2C lcd(0x27, LCD_COLS, LCD_ROWS);
CharacterDisplayRenderer renderer(new LiquidCrystal_I2CAdapter(&lcd), LCD_COLS, LCD_ROWS);
LcdMenu menu(renderer);

// joystick pins: A0 horizontal, A1 vertical
// horizontal movement is mapped to BACK/RIGHT by default
JoystickAdapter joystick(&menu, A0, A1);
Button joyButton(2);
ButtonAdapter joyButtonA(&menu, &joyButton, ENTER);

void setup() {
    Serial.begin(9600);
    renderer.begin();
    joyButton.begin();
    menu.setScreen(mainScreen);
}

void loop() {
    joystick.observe();
    joyButtonA.observe();
}
