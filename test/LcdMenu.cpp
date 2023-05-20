#include <ArduinoUnitTests.h>
#include <ItemCommand.h>
#include <ItemInput.h>
#include <ItemList.h>
#include <ItemToggle.h>

#define LCD_ROWS 2
#define LCD_COLS 16

MAIN_MENU(ITEM_INPUT("Random", NULL), ITEM_INPUT("Connect", NULL),
          ITEM_BASIC("Settings"), ITEM_COMMAND("Backlight", NULL),
          ITEM_TOGGLE("Toggle", "ON", "OFF", NULL), ITEM_BASIC("Blink random"));

unittest(text_set_correctly) { assertEqual("Random", mainMenu[1]->getText()); }

unittest(no_text_off_on_for_item_input) {
    assertNull(mainMenu[1]->getTextOn());
    assertNull(mainMenu[1]->getTextOff());
}

unittest(text_off_on_for_item_toggle) {
    assertEqual("ON", mainMenu[5]->getTextOn());
    assertEqual("OFF", mainMenu[5]->getTextOff());
}

unittest(check_types) {
    assertEqual(MENU_ITEM_MAIN_MENU_HEADER, mainMenu[0]->getType());
    assertEqual(MENU_ITEM_INPUT, mainMenu[1]->getType());
    assertEqual(MENU_ITEM_NONE, mainMenu[3]->getType());
    assertEqual(MENU_ITEM_COMMAND, mainMenu[4]->getType());
    assertEqual(MENU_ITEM_TOGGLE, mainMenu[5]->getType());
}

unittest_main()