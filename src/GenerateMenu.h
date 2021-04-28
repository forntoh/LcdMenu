#ifndef PLAYER_H
#define _H

#include <MenuItem.h>

uint8_t index = 0;

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

MenuItem* generateMenu(char* input) {
    uint8_t step = 0;
    uint8_t prevPos;

    MenuItem* currMenu;

    String line;
    while ((line = readLine(input)).length() > 0) {

        uint8_t pos = line.charAt(0) - '0';
        uint8_t size = line.charAt(1) - '0';
        uint8_t i = line.charAt(2) - '0';
        uint8_t type = line.charAt(3) - '0';

        if (type == 0)
            currMenu = creatMenu(size, NULL);
        else if (type == MENU_ITEM_SUB_MENU) {
            prevPos = pos + 1;

            if (i == 1)
                currMenu[step][prevPos] =
                    ItemSubMenu(line.substring(4), creatMenu(size, currMenu));
            else
                currMenu[pos + 1] =
                    ItemSubMenu(line.substring(4), creatMenu(size, currMenu));

        } else {
            if (pos == 0) ++step;

            if (i == 2)
                currMenu[step - 1][prevPos][pos + 1] =
                    MenuItem(line.substring(4));
            else
                currMenu[step][pos + 1] = MenuItem(line.substring(4));
        }
    }
    index = 0;
    return currMenu;
}

#endif