/*
  GenerateMenu.h - Main include file for the LcdMenu Menu Generator

  MIT License

  Copyright (c) 2020 Forntoh Thomas

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

#ifndef MENU_GENERATOR_H
#define MENU_GENERATOR_H

#include <MenuItem.h>

/**
 * # Menu Generator
 *
 * Used to generate menu's from strings.
 *
 * ## Todos
 *
 * - Get parameters from the line
 *
 * - Use a map to for `ftpr` (string name to function map)
 */

/**
 * ## Methods
 */

/**
 * Current character position
 */
uint8_t index = 0;

/**
 * Create a new menu and assign it's parent
 * @param size number of items in the menu to create
 * @param parent parent for the new menu
 * @return `MenuItem*` the created menu
 */
MenuItem* creatMenu(uint8_t size, MenuItem* parent) {
    MenuItem* menu = new MenuItem[size + 2];
    menu[0] = parent != NULL ? ItemHeader(parent) : ItemHeader();
    menu[size + 1] = ItemFooter();
    return menu;
}

/**
 * Read a single line separated by `\n`
 * @param file input to read
 * @return `char*` the line read
 */
char* readLine(char* file) {
    uint8_t i = 0;
    bool EOL = false;
    static char line[30];

    while (!EOL) {
        char c = file[index];
        switch (c) {
            case '\0':
                return NULL;
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
    return line;
}

uint8_t n(char* line, uint8_t start, char delimiter) {
    uint8_t n = start;
    while (line[n] != delimiter) n++;
    return n;
}

/**
 * Generate a menu from a string input
 * @param input number of items in the menu to create
 * @return `MenuItem*` the created menu
 */
MenuItem* generateMenu(char* input) {
    MenuItem* currMenu;

    char* line;
    while ((line = readLine(input)) != NULL) {
        uint8_t count = 0;
        while (line[count] - '0' < 10 && count < strlen(line)) count++;

        uint8_t pos = line[0] - '0';
        uint8_t size = line[1] - '0';
        uint8_t type = line[count - 1] - '0';

        if (type == 0)
            currMenu = creatMenu(size, NULL);
        else {
            uint8_t p = n(line, count + 1, '\0');
            char* name = new char[p - 2];

            strncpy(name, line + count, p - 3);
            name[p - 2] = '\0';

            if (type == MENU_ITEM_SUB_MENU)
                if (count == 3)
                    currMenu[pos + 1] =
                        ItemSubMenu(name, creatMenu(size, currMenu));
                else {
                    MenuItem item = currMenu[line[2] - '0' + 1];
                    for (uint8_t m = 3; m < count - 1; m++)
                        item = item[line[m] - '0' + 1];
                    item[pos + 1] =
                        ItemSubMenu(name, creatMenu(size, currMenu));
                }
            else {
                MenuItem item = currMenu[line[1] - '0' + 1];
                for (uint8_t m = 2; m < count - 1; m++)
                    item = item[line[m] - '0' + 1];
                item[pos + 1] = MenuItem(name);
            }
        }
    }
    index = 0;
    return currMenu;
}

#endif