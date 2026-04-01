import os
import re
import sys


def button_press_template(button_name):
    return (
        "  - set-control:\n"
        f"      part-id: {button_name}\n"
        "      control: pressed\n"
        "      value: 1\n"
    )


def button_release_template(button_name):
    return (
        "  - set-control:\n"
        f"      part-id: {button_name}\n"
        "      control: pressed\n"
        "      value: 0\n"
        f"  - delay: {wait_time_after_release}ms\n"
    )


def button_down_template(button_name):
    return (
        "  - set-control:\n"
        f"      part-id: {button_name}\n"
        "      control: pressed\n"
        "      value: 1\n"
        f"  - delay: {hold_time}ms\n"
    )


def button_up_template(button_name):
    return (
        "  - set-control:\n"
        f"      part-id: {button_name}\n"
        "      control: pressed\n"
        "      value: 0\n"
        f"  - delay: {wait_time_after_release}ms\n"
    )


def replace_lines(file_path, compiled_replacements):
    total_wait_time = 0
    button_by_name = {
        "upButton": "btn1",
        "downButton": "btn2",
        "enterButton": "btn3",
        "backButton": "btn4",
        "leftButton": "btn5",
        "rightButton": "btn6",
        "backspaceButton": "btn7",
    }
    pending_release = ""
    with open(file_path, "r") as file:
        lines = file.readlines()

    with open(file_path, "w") as file:
        for line in lines:
            line = re.sub(
                r'^(\s*-\s*wait-serial:\s*")#LOG#\s*',
                r"\1",
                line,
            )

            press_match = re.match(r"^\s*-\s*simulate:\s*(\w+)-press\s*$", line)
            if press_match:
                button_name = press_match.group(1)
                button_id = button_by_name.get(button_name)
                if button_id:
                    line = button_press_template(button_id) + "\n"
                    pending_release = button_id
                    total_wait_time += serial_wait_time
                    file.write(line)
                    continue

            for regex, (replacement, wait_time) in compiled_replacements:
                if regex.search(line):
                    if pending_release:
                        line = (
                            button_release_template(pending_release)
                            + "\n"
                            + replacement
                            + "\n"
                        )
                        total_wait_time += wait_time_after_release
                        pending_release = ""
                    else:
                        line = replacement + "\n"
                    total_wait_time += wait_time
                    break

            if re.match(r"^(\s*)-\s*wait-serial:", line):
                if pending_release:
                    if not line.endswith("\n"):
                        line += "\n"
                    line += button_release_template(pending_release)
                    total_wait_time += wait_time_after_release
                    pending_release = ""

            file.write(line)

        if pending_release:
            file.write(button_release_template(pending_release))
            total_wait_time += wait_time_after_release
    return total_wait_time


if __name__ == "__main__":
    if len(sys.argv) != 5:
        print(
            "Usage: python prepare_workflow.py <file_path> <serial_wait_time> <wait_time_after_release> <press_holding_time>"
        )
        sys.exit(1)

    file_path = sys.argv[1]
    serial_wait_time = int(sys.argv[2])
    press_holding_time = int(sys.argv[3])
    wait_time_after_release = int(sys.argv[4])

    hold_time = press_holding_time * 10

    down_wait = hold_time + serial_wait_time
    up_wait = wait_time_after_release + serial_wait_time

    replacements = {
        r".*- simulate: upButton-down": (button_down_template("btn1"), down_wait),
        r".*- simulate: upButton-up": (button_up_template("btn1"), up_wait),
        r".*- simulate: downButton-down": (button_down_template("btn2"), down_wait),
        r".*- simulate: downButton-up": (button_up_template("btn2"), up_wait),
        r".*- simulate: enterButton-down": (button_down_template("btn3"), down_wait),
        r".*- simulate: enterButton-up": (button_up_template("btn3"), up_wait),
        r".*- simulate: backButton-down": (button_down_template("btn4"), down_wait),
        r".*- simulate: backButton-up": (button_up_template("btn4"), up_wait),
        r".*- simulate: leftButton-down": (button_down_template("btn5"), down_wait),
        r".*- simulate: leftButton-up": (button_up_template("btn5"), up_wait),
        r".*- simulate: rightButton-down": (button_down_template("btn6"), down_wait),
        r".*- simulate: rightButton-up": (button_up_template("btn6"), up_wait),
        r".*- simulate: backspaceButton-down": (
            button_down_template("btn7"),
            down_wait,
        ),
        r".*- simulate: backspaceButton-up": (button_up_template("btn7"), up_wait),
    }

    # Pre-compile regex patterns for efficiency
    compiled_replacements = [
        (re.compile(pattern), replacement_tuple)
        for pattern, replacement_tuple in replacements.items()
    ]

    total_wait_time = replace_lines(file_path, compiled_replacements)

    print(total_wait_time)
