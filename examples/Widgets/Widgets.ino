#include <Button.h>
#include <ItemWidget.h>
#include <LcdMenu.h>
#include <MenuScreen.h>
#include <display/LiquidCrystal_I2CAdapter.h>
#include <input/ButtonAdapter.h>
#include <renderer/CharacterDisplayRenderer.h>
#include <widget/WidgetBool.h>
#include <widget/WidgetList.h>
#include <widget/WidgetRange.h>

#define LCD_ROWS 2
#define LCD_COLS 16
#define LCD_ADDR 0x27

// Custom characters
byte plusMinus[8] = {B00000, B00100, B01110, B00100, B00000, B01110, B00000, B00000};  // ±
byte euro[8] = {B00111, B01000, B11110, B01000, B11110, B01000, B00111, B00000};       // €

std::vector<const char*> options = {"Buy", "Sell"};

MENU_SCREEN(
    mainScreen,
    mainItems,
    ITEM_WIDGET(
        "Auto",
        [](const uint8_t option, bool isAuto) { Serial.println(options[option]); Serial.println(isAuto); },
        WIDGET_LIST(options, 0, "%s", 0, true),
        WIDGET_BOOL(false, "Yes", "No", ",%s")),
    ITEM_WIDGET(
        "Price",
        [](int price) { Serial.println(price); },
        WIDGET_RANGE(10, 5, 0, 1000, "%.1d\003", 1)),
    ITEM_WIDGET(
        "Quantity",
        [](float quantity, int tolerance) { Serial.println(quantity); Serial.println(tolerance); },
        WIDGET_RANGE(1.0f, 0.1f, 0.1f, 100.0f, "%.1f", 0),
        WIDGET_RANGE(10, 1, 0, 100, "\002%d%%", 1)));

LiquidCrystal_I2C lcd(LCD_ADDR, LCD_COLS, LCD_ROWS);
LiquidCrystal_I2CAdapter lcdAdapter(&lcd);
CharacterDisplayRenderer renderer(&lcdAdapter, LCD_COLS, LCD_ROWS);
LcdMenu menu(renderer);
Button upBtn(5);
ButtonAdapter upBtnA(&menu, &upBtn, UP);
Button downBtn(6);
ButtonAdapter downBtnA(&menu, &downBtn, DOWN);
Button enterBtn(7);
ButtonAdapter enterBtnA(&menu, &enterBtn, ENTER);
Button backBtn(8);
ButtonAdapter backBtnA(&menu, &backBtn, BACK);
Button leftBtn(9);
ButtonAdapter leftBtnA(&menu, &leftBtn, LEFT);
Button rightBtn(10);
ButtonAdapter rightBtnA(&menu, &rightBtn, RIGHT);
Button backspaceBtn(11);
ButtonAdapter backspaceBtnA(&menu, &backspaceBtn, BACKSPACE);

void setup() {
    upBtn.begin();
    downBtn.begin();
    enterBtn.begin();
    backBtn.begin();
    leftBtn.begin();
    rightBtn.begin();
    backspaceBtn.begin();
    Serial.begin(9600);
    Serial.println("WIDGETS_BOOT_OK");
    renderer.begin();
    menu.setScreen(mainScreen);
    lcd.createChar(2, plusMinus);
    lcd.createChar(3, euro);
}

void loop() {
    upBtnA.observe();
    downBtnA.observe();
    enterBtnA.observe();
    backBtnA.observe();
    leftBtnA.observe();
    rightBtnA.observe();
    backspaceBtnA.observe();
}
