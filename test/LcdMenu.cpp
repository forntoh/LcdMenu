#include <ArduinoUnitTests.h>
#include <ItemCommand.h>
#include <ItemInput.h>
#include <ItemList.h>
#include <ItemToggle.h>

#define LCD_ROWS 2
#define LCD_COLS 16

#define ITEM_MAIN_HEADER_INDEX 0
#define ITEM_INPUT_INDEX 1
#define ITEM_COMMAND_INDEX 4
#define ITEM_TOGGLE_INDEX 5
#define ITEM_LIST_INDEX 99

void commandCallback() {}
void toggleCallback(uint8_t) {}

MAIN_MENU(ITEM_INPUT("Random", NULL), ITEM_INPUT("Connect", NULL),
          ITEM_BASIC("Settings"), ITEM_COMMAND("Backlight", commandCallback),
          ITEM_TOGGLE("Toggle", "ON", "OFF", toggleCallback),
          ITEM_BASIC("Blink random"));

unittest(text_set_correctly) {
    assertEqual("Random", mainMenu[ITEM_INPUT_INDEX]->getText());
}

unittest(no_text_off_on_for_item_input) {
    assertNull(mainMenu[ITEM_INPUT_INDEX]->getTextOn());
    assertNull(mainMenu[ITEM_INPUT_INDEX]->getTextOff());
}

unittest(text_off_on_for_item_toggle) {
    assertEqual("ON", mainMenu[ITEM_TOGGLE_INDEX]->getTextOn());
    assertEqual("OFF", mainMenu[ITEM_TOGGLE_INDEX]->getTextOff());
}

unittest(check_types) {
    assertEqual(MENU_ITEM_MAIN_MENU_HEADER,
                mainMenu[ITEM_MAIN_HEADER_INDEX]->getType());
    assertEqual(MENU_ITEM_INPUT, mainMenu[ITEM_INPUT_INDEX]->getType());
    assertEqual(MENU_ITEM_NONE, mainMenu[3]->getType());
    assertEqual(MENU_ITEM_COMMAND, mainMenu[ITEM_COMMAND_INDEX]->getType());
    assertEqual(MENU_ITEM_TOGGLE, mainMenu[ITEM_TOGGLE_INDEX]->getType());
}

unittest(can_set_input_value) {
    char* expected = "TEST";
    assertEqual("", mainMenu[ITEM_INPUT_INDEX]->getValue());
    mainMenu[ITEM_INPUT_INDEX]->setValue(expected);
    assertEqual(expected, mainMenu[ITEM_INPUT_INDEX]->getValue());
}

unittest_main()