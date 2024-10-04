#include <Button.h>
#include <Control.h>
#include <ItemControl.h>
#include <ItemInputCharset.h>
#include <ItemList.h>
#include <ItemToggle.h>
#include <LcdMenu.h>
#include <MenuScreen.h>
#include <SimpleRotary.h>
#include <display/LiquidCrystal_I2CAdapter.h>
#include <input/ButtonAdapter.h>
#include <input/KeyboardAdapter.h>
#include <input/SimpleRotaryAdapter.h>

#define LCD_ROWS 2
#define LCD_COLS 16

// Initialize the main menu items
// clang-format off
MENU_SCREEN(mainScreen, mainItems,
    new ItemControl<float>("Temp", 
        new NumberControl<float>({.min=-30.0, .max=40.0, .initial=26.5, .step=0.5, .format="%+2.1f\002C", .cycle=false, .blinkerOffset=2}),
        [](float v0) {
            Serial.print("callback1(");
            Serial.print(v0);
            Serial.println(")");
        }),
    new ItemControl<float>("Price", 
        new NumberControl<float>({.min=0.0, .max=10.0, .initial=1.0, .step=0.1, .format="$%2.2f", .cycle=true}),
        [](float v0) {
            Serial.print("callback1(");
            Serial.print(v0);
            Serial.println(")");
        }),
    new ItemControl<float, int>("Dist", 
        new NumberControl<float>({.min=0.0, .max=10.0, .initial=1.0, .step=0.1, .format="%2.1fm", .cycle=false, .blinkerOffset=1}),
        new NumberControl<int>({.min=0, .max=100, .initial=10, .step=1, .format="\003%d%%", .cycle=false, .blinkerOffset=1}),
        [](float v0, int v1) {
            Serial.print("callback2(");
            Serial.print(v0);
            Serial.print(", ");
            Serial.print(v1);
            Serial.println(")");
        }),
    new ItemControl<int, int, int>("Time", 
        new NumberControl<int>({.min=0, .max=23, .initial=0, .step=1, .format="%02d", .cycle=true}),
        new NumberControl<int>({.min=0, .max=59, .initial=0, .step=1, .format=":%02d", .cycle=true}),
        new NumberControl<int>({.min=0, .max=59, .initial=0, .step=1, .format=":%02d", .cycle=true}),
        [](int v0, int v1, int v2) {
            Serial.print("callback3(");
            Serial.print(v0);
            Serial.print(", ");
            Serial.print(v1);
            Serial.print(", ");
            Serial.print(v2);
            Serial.println(")");
        }),
    ITEM_BASIC("Blink random"));
// clang-format on

// Construct the LcdMenu
LiquidCrystal_I2C lcd(0x27, LCD_COLS, LCD_ROWS);
LiquidCrystal_I2CAdapter lcdAdapter(&lcd, LCD_COLS, LCD_ROWS);
LcdMenu menu(lcdAdapter);
SimpleRotary encoder(2, 3, 4);
SimpleRotaryAdapter rotaryInput(&menu, &encoder);  // Rotary input adapter
Button backspaceBtn(5);
ButtonAdapter backspaceBtnA(&menu, &backspaceBtn, BACKSPACE);  // Push button for backspace
KeyboardAdapter keyboard(&menu, &Serial);

void setup() {
    backspaceBtn.begin();
    lcdAdapter.begin();
    Serial.begin(9600);
    menu.setScreen(mainScreen);
    byte celsius[8] = {B11100, B10100, B11100, B0000, B00000, B00000, B00000, B00000};
    byte plusMinus[8] = {B00000, B00100, B01110, B00100, B00000, B01110, B00000, B00000};
    lcd.createChar(2, celsius);
    lcd.createChar(3, plusMinus);
}

void loop() {
    rotaryInput.observe();
    backspaceBtnA.observe();
    keyboard.observe();
}
