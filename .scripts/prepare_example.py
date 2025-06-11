import os
import re
import sys
import shutil


def replace_lines(file_path, replacements):
    with open(file_path, "r") as file:
        lines = file.readlines()

    seen = set()
    new_lines = []

    for line in lines:
        replaced = False
        for pattern, (replacement, key) in replacements.items():
            if re.search(pattern, line):
                if key not in seen:
                    new_lines.append(replacement + "\n")
                    seen.add(key)
                replaced = True
                break
        if not replaced:
            new_lines.append(line)

    with open(file_path, "w") as file:
        file.writelines(new_lines)

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python prepare_example.py <file_path>")
        sys.exit(1)

    file_path = sys.argv[1]

    # Change the file extension to .cpp but keep the original file intact
    base = os.path.splitext(file_path)[0]
    new_file_path = base + ".cpp"
    shutil.copyfile(file_path, new_file_path)

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
        r"#include <(Button.h|input/ButtonAdapter.h)>": (replacement1, "includes"),
        r"^\s*Button(?:Adapter)? .*\);": (replacement2, "buttons"),
        r".*\.observe\(\);": (replacement3, "observes"),
        r"^\s*(?:upBtn|downBtn|enterBtn|backBtn|leftBtn|rightBtn|backspaceBtn)\.begin\(\);|Serial.begin\(.*\);": (replacement4, "setup"),
        r"#include <LcdMenu.h>": (replacement5, "menu")
    }

    replace_lines(new_file_path, replacements)

    # Print the new path
    print(new_file_path)