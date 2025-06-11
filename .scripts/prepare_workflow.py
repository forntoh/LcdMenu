import os
import re
import sys


def button_press_template(button_name):
    return f"""
  - set-control:
      part-id: {button_name}
      control: pressed
      value: 1
  - delay: {press_holding_time}ms
  - set-control:
      part-id: {button_name}
      control: pressed
      value: 0
  - delay: {wait_time_after_release}ms
  """


def button_down_template(button_name):
    return f"""
  - set-control:
      part-id: {button_name}
      control: pressed
      value: 1
  - delay: {hold_time}ms
  """


def button_up_template(button_name):
    return f"""
  - set-control:
      part-id: {button_name}
      control: pressed
      value: 0
  - delay: {wait_time_after_release}ms
  """

def replace_lines(file_path, replacements):
    total_wait_time = 0
    with open(file_path, "r") as file:
        lines = file.readlines()

    with open(file_path, "w") as file:
        for line in lines:
            replaced = False
            for pattern, (replacement, wait_time) in replacements.items():
                if re.search(pattern, line):
                    line = replacement + "\n"
                    total_wait_time += wait_time
                    replaced = True
                    break
            file.write(line)
    
    return total_wait_time

if __name__ == "__main__":
    if len(sys.argv) != 5:
        print("Usage: python prepare_workflow.py <file_path> <serial_wait_time> <wait_time_after_release> <press_holding_time>")
        sys.exit(1)

    file_path = sys.argv[1]
    serial_wait_time = int(sys.argv[2])
    press_holding_time = int(sys.argv[3])
    wait_time_after_release = int(sys.argv[4])

    hold_time = press_holding_time * 10

    press_wait = press_holding_time + wait_time_after_release + serial_wait_time
    down_wait = hold_time + serial_wait_time
    up_wait = wait_time_after_release + serial_wait_time

    replacements = {
        r".*- simulate: upButton-press": (button_press_template("btn1"), press_wait),
        r".*- simulate: upButton-down": (button_down_template("btn1"), down_wait),
        r".*- simulate: upButton-up": (button_up_template("btn1"), up_wait),

        r".*- simulate: downButton-press": (button_press_template("btn2"), press_wait),
        r".*- simulate: downButton-down": (button_down_template("btn2"), down_wait),
        r".*- simulate: downButton-up": (button_up_template("btn2"), up_wait),

        r".*- simulate: enterButton-press": (button_press_template("btn3"), press_wait),
        r".*- simulate: enterButton-down": (button_down_template("btn3"), down_wait),
        r".*- simulate: enterButton-up": (button_up_template("btn3"), up_wait),

        r".*- simulate: backButton-press": (button_press_template("btn4"), press_wait),
        r".*- simulate: backButton-down": (button_down_template("btn4"), down_wait),
        r".*- simulate: backButton-up": (button_up_template("btn4"), up_wait),

        r".*- simulate: leftButton-press": (button_press_template("btn5"), press_wait),
        r".*- simulate: leftButton-down": (button_down_template("btn5"), down_wait),
        r".*- simulate: leftButton-up": (button_up_template("btn5"), up_wait),

        r".*- simulate: rightButton-press": (button_press_template("btn6"), press_wait),
        r".*- simulate: rightButton-down": (button_down_template("btn6"), down_wait),
        r".*- simulate: rightButton-up": (button_up_template("btn6"), up_wait),

        r".*- simulate: backspaceButton-press": (button_press_template("btn7"), press_wait),
        r".*- simulate: backspaceButton-down": (button_down_template("btn7"), down_wait),
        r".*- simulate: backspaceButton-up": (button_up_template("btn7"), up_wait),
    }

    total_wait_time = replace_lines(file_path, replacements)

    print(total_wait_time)
