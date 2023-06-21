import re

import click


def parse_output(output):
    # Extract RAM percentage
    ram_percentage = re.search(r"RAM:\s+\[+.*\]\s+(\d+\.*\d*)%", output).group(1)

    # Extract Flash percentage
    flash_percentage = re.search(r"Flash:\s+\[+.*\]\s+(\d+\.*\d*)%", output).group(1)

    result = {
        "ram": float(ram_percentage),
        "flash": float(flash_percentage)
    }

    return result

@click.command()
@click.argument("filename", required=False, type=click.File("r"), default="-")
def main(filename):
    output = filename.read()

    result = parse_output(output)
    click.echo(result)

if __name__ == "__main__":
    main()
