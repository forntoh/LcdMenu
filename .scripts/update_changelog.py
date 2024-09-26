import re

import requests

GITHUB_API_URL = "https://api.github.com/repos/{owner}/{repo}/releases"
OWNER = "forntoh"
REPO = "LcdMenu"
CHANGELOG_FILE = "docs/source/reference/changelog.rst"

def fetch_releases():
    url = GITHUB_API_URL.format(owner=OWNER, repo=REPO)
    response = requests.get(url)
    response.raise_for_status()
    return response.json()

def convert_md_to_rst(body):
    lines = body.split('\n')
    rst_lines = []
    for line in lines:
        if line.startswith('## '):
            heading = line[3:]
            rst_lines.append(heading)
            rst_lines.append('~' * len(heading))
        elif line.startswith('### '):
            heading = line[4:]
            rst_lines.append(heading)
            rst_lines.append('+' * len(heading))
        else:
            line = re.sub(r'(https://github\.com/forntoh/LcdMenu/pull/(\d+))', r'`#\2 <\1>`_', line)
            rst_lines.append(line)
    return '\n'.join(rst_lines)

def format_release(release):
    body = convert_md_to_rst(release['body'])
    return f"{release['name']}\n{'-' * len(release['name'])}\n\n{body}\n"


def update_changelog(releases):
    with open(CHANGELOG_FILE, 'r') as file:
        lines = file.readlines()

    with open(CHANGELOG_FILE, 'w') as file:
        for line in lines:
            if line.strip() == "----------------":
                break
            file.write(line)
        
        file.write("\n")
        for release in releases:
            file.write(format_release(release))
            print(f"Added release: {release['name']}")
            file.write("\n")

if __name__ == "__main__":
    releases = fetch_releases()
    update_changelog(releases)