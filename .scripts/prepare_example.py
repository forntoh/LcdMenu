import os
import re
import sys


def replace_lines(file_path, replacements):
    with open(file_path, 'r') as file:
        lines = file.readlines()

    with open(file_path, 'w') as file:
        for line in lines:
            for pattern, replacement in replacements.items():
                if re.search(pattern, line):
                    line = replacement + '\n'
            file.write(line)

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python prepare_example.py <file_path>")
        sys.exit(1)

    file_path = sys.argv[1]

    # Change the file extension to .cpp
    base = os.path.splitext(file_path)[0]
    new_file_path = base + '.cpp'
    os.rename(file_path, new_file_path)

    replacement1 = """
#include <Button.h>
#include <input/ButtonAdapter.h>"""

    replacement2 = """
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
ButtonAdapter backspaceBtnA(&menu, &backspaceBtn, BACKSPACE);"""

    replacement3 = """
    upBtnA.observe();
    downBtnA.observe();
    enterBtnA.observe();
    backBtnA.observe();
    leftBtnA.observe();
    rightBtnA.observe();
    backspaceBtnA.observe();"""

    replacement4 = """
    upBtn.begin();
    downBtn.begin();
    enterBtn.begin();
    backBtn.begin();
    leftBtn.begin();
    rightBtn.begin();
    backspaceBtn.begin();
    Serial.begin(9600);"""

    replacement5 = """
#define DISPLAY_TIMEOUT 2000
#include <LcdMenu.h>"""

    replacements = {
        r"#include <input/.*Adapter.h>": replacement1,
        r".*Adapter .*\(&menu, .*\);": replacement2,
        r".*\.observe\(\);": replacement3,
        r".*Serial.begin\(.*\);": replacement4,
        r"#include <LcdMenu.h>": replacement5
    }

    replace_lines(new_file_path, replacements)

    # Print the new path
    print(new_file_path)