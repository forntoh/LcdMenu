#include <ArduinoUnitTests.h>
#include <ItemCommand.h>
#include <ItemInput.h>
#include <ItemList.h>
#include <ItemToggle.h>
#include <MenuScreen.h>

#define LCD_ROWS 2
#define LCD_COLS 16

#define ITEM_MAIN_HEADER_INDEX 0
#define ITEM_INPUT_INDEX 0
#define ITEM_COMMAND_INDEX 3
#define ITEM_TOGGLE_INDEX 4
#define ITEM_LIST_INDEX 99

void commandCallback() {}
void toggleCallback(uint16_t) {}

// clang-format off
MENU_SCREEN(mainScreen, mainItems,
    ITEM_INPUT("Random", NULL),
    ITEM_INPUT("Connect", NULL),
    ITEM_BASIC("Settings"),
    ITEM_COMMAND("Backlight", commandCallback),
    ITEM_TOGGLE("Toggle", "ON", "OFF", toggleCallback),
    ITEM_BASIC("Blink random"));
// clang-format on

unittest(text_set_correctly) {
    assertEqual("Random", mainItems[ITEM_INPUT_INDEX]->getText());
}

unittest(text_off_on_for_item_toggle) {
    assertEqual("ON", (static_cast<ItemToggle*>(mainItems[ITEM_TOGGLE_INDEX]))->getTextOn());
    assertEqual("OFF", (static_cast<ItemToggle*>(mainItems[ITEM_TOGGLE_INDEX]))->getTextOff());
}

unittest(can_set_input_value) {
    char* expected = "TEST";
    assertEqual("", (static_cast<ItemInput*>(mainItems[ITEM_INPUT_INDEX]))->getValue());
    (static_cast<ItemInput*>(mainItems[ITEM_INPUT_INDEX]))->setValue(expected);
    assertEqual(expected, (static_cast<ItemInput*>(mainItems[ITEM_INPUT_INDEX]))->getValue());
}

unittest_main()
