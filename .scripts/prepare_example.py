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
ButtonAdapter upBtn(&menu, Button(5), UP);
ButtonAdapter downBtn(&menu, Button(6), DOWN);
ButtonAdapter enterBtn(&menu, Button(7), ENTER);
ButtonAdapter backBtn(&menu, Button(8), BACK);"""

    replacement3 = """
    upBtn.observe();
    downBtn.observe();
    enterBtn.observe();
    backBtn.observe();"""

    replacement4 = """
    upBtn.begin();
    downBtn.begin();
    enterBtn.begin();
    backBtn.begin();
    Serial.begin(9600);"""

    replacements = {
        r"#include <input/.*Adapter.h>": replacement1,
        r".*Adapter .*\(&menu, .*\);": replacement2,
        r".*\.observe\(\);": replacement3,
        r".*Serial.begin\(.*\);": replacement4
    }

    replace_lines(new_file_path, replacements)

    # Print the new path
    print(new_file_path)