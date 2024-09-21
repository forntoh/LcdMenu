import os
import re
import sys

press_holding_time = 100

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

def replace_lines(file_path, replacements, serial_wait_time, wait_time_after_release):
    total_wait_time = 0
    with open(file_path, 'r') as file:
        lines = file.readlines()

    with open(file_path, 'w') as file:
        for line in lines:
            for pattern, replacement in replacements.items():
                if re.search(pattern, line):
                    line = replacement + '\n'
                    total_wait_time += press_holding_time + wait_time_after_release + serial_wait_time
            file.write(line)
    
    return total_wait_time

if __name__ == "__main__":
    if len(sys.argv) != 4:
        print("Usage: python prepare_workflow.py <file_path> <serial_wait_time>")
        sys.exit(1)

    file_path = sys.argv[1]
    serial_wait_time = int(sys.argv[2])
    wait_time_after_release = int(sys.argv[3])

    replacements = {
        r".*- simulate: upButton-press": button_press_template("btn1"),
        r".*- simulate: downButton-press": button_press_template("btn2"),
        r".*- simulate: enterButton-press": button_press_template("btn3"),
        r".*- simulate: backButton-press": button_press_template("btn4"),
        r".*- simulate: leftButton-press": button_press_template("btn5"),
        r".*- simulate: rightButton-press": button_press_template("btn6"),
        r".*- simulate: backspaceButton-press": button_press_template("btn7"),
    }

    total_wait_time = replace_lines(file_path, replacements, serial_wait_time, wait_time_after_release)

    print(total_wait_time)