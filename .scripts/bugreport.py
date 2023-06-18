import re

import click

search = r'(\s+- \d\.\d\.\d)'
filename = ".github/ISSUE_TEMPLATE/bug_report.yml"

@click.command()
@click.argument('version')

def build(version):
    with open(filename, "r") as file:
        contents = file.read()
        old_version = re.search(search, contents)[0]
        updated_contents = re.sub(old_version, f'\n        - {version}{old_version}', contents)

    with open(filename, "w") as file:
        file.write(updated_contents)
        file.truncate()
        print("Updated version number in bug report template")
    
if __name__ == '__main__':
    build()