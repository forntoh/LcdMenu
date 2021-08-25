import re
import click

@click.command()
@click.argument('files', type=click.Path("r"), nargs=-1)
@click.argument('version')

def build(files, version):

    for file in files:
        data = ''
        separator = ''
        if file.endswith('.json'): separator = ": "
        else: separator = '='

        with open(file, "r") as a_file:
            for line in a_file:
                stripped_line = line.strip()
                key = stripped_line.split(separator, 1)[0]

                if re.search("version", key) != None:
                    anchor = ''
                    suffix = ''
                    prefix = ''
                    if separator == ': ':
                        anchor = '"'
                        suffix = ','
                        prefix = '  '
                    data+=(prefix + key + separator + anchor + version + anchor + suffix + '\n')
                else:
                    data+=line

        with open(file, "w") as a_file:
            a_file.truncate()
            a_file.write(data)
            print("Done")
    
if __name__ == '__main__':
    build()