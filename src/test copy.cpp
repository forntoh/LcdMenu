#include <Arduino.h>
#include <LcdMenu.h>
String readLine(char* file);
MenuItem* creatMenu(uint8_t size, MenuItem* parent);

uint8_t index = 0;

void setup() {
    Serial.begin(9600);
    while (!Serial) continue;

    // char input[] =
    //     "002\n032Settings\n070Backlight\n160Contrast\n132WiFi\n050User\n152"
    //     "Android01\n";

    // MenuItem* nextMenu;
    // MenuItem* currMenu;

    // String line;
    // while ((line = readLine(input)).length() > 0) {
    //     uint8_t pos = line.charAt(0) - '0';
    //     uint8_t type = line.charAt(1) - '0';
    //     uint8_t size = line.charAt(2) - '0';
    //     if (type == 0) {
    //         currMenu = creatMenu(size, NULL);
    //     } else if (type == MENU_ITEM_SUB_MENU) {
    //         nextMenu = creatMenu(size, currMenu);
    //         currMenu[pos + 1] = ItemSubMenu(line.substring(3), nextMenu);
    //     } else {
    //         nextMenu[pos + 1] = MenuItem(line.substring(3));
    //     }
    // }

    char input[] =
        "0020\n0320Settings\n1320WiFi\n0701Backlight\n1601Contrast\n0501User\n1"
        "321Android\n0502Password\n1402Connect\n";

    uint8_t step = 0;
    uint8_t prevPos;

    MenuItem* currMenu;

    String line;
    while ((line = readLine(input)).length() > 0) {
        uint8_t pos = line.charAt(0) - '0';
        uint8_t type = line.charAt(1) - '0';
        uint8_t size = line.charAt(2) - '0';
        uint8_t i = line.charAt(3) - '0';

        // Serial.println(step, DEC);

        if (type == 0) {
            currMenu = creatMenu(size, NULL);
        } else if (type == MENU_ITEM_SUB_MENU) {
            prevPos = pos + 1;

            if (i == 1) {
                currMenu[step].getSubMenu()[prevPos] =
                    ItemSubMenu(line.substring(4), creatMenu(size, currMenu));
            } else
                currMenu[pos + 1] =
                    ItemSubMenu(line.substring(4), creatMenu(size, currMenu));

        } else {
            if (pos == 0) ++step;

            if (i == 2) {
                currMenu[step - 1].getSubMenu()[prevPos].getSubMenu()[pos + 1] =
                    MenuItem(line.substring(4));
            } else
                currMenu[step].getSubMenu()[pos + 1] =
                    MenuItem(line.substring(4));
        }
    }

    for (uint8_t i = 0; i < 4; i++) {
        MenuItem item1 = currMenu[i];
        Serial.println(item1.getText());

        if (item1.getType() == MENU_ITEM_SUB_MENU) {
            for (uint8_t j = 0; j < 4; j++) {
                MenuItem item2 = item1.getSubMenu()[j];
                Serial.print(' ');
                Serial.println(item2.getText());

                if (item2.getType() == MENU_ITEM_SUB_MENU) {
                    for (uint8_t k = 0; k < 4; k++) {
                        MenuItem item3 = item2.getSubMenu()[k];
                        Serial.print(F("  "));
                        Serial.println(item3.getText());
                    }
                }
            }
        }
    }
}

MenuItem* creatMenu(uint8_t size, MenuItem* parent) {
    MenuItem* menu = new MenuItem[size + 2];
    menu[0] = parent != NULL ? ItemHeader(parent) : ItemHeader();
    menu[size + 1] = ItemFooter();
    return menu;
}

String readLine(char* file) {
    uint8_t i = 0;
    bool EOL = false;
    static char line[35];

    while (!EOL) {
        char c = file[index];
        switch (c) {
            case '\0':
                return String("");
            case '\n':
                line[i] = 0;
                i = 0;
                EOL = true;
                break;
            default:
                line[i] = c;
                i++;
                break;
        }
        index++;
    }
    return String(line);
}

void loop() {}