import re
import click

prefixes = [':', '*', '}', '/', '#', 'for ', 'if ', 'switch ', 'case ', 'return ', 'class ', 'explicit ']
sufixes = [';', '-', '=', ',', '*', '.', '= {', '{}']

pre_key_1 = """#######################################
# Syntax Coloring Map
#######################################

#######################################
# Datatypes (KEYWORD1)
#######################################

"""

pre_key_2 = """
#######################################
# Methods and Functions (KEYWORD2)
#######################################

"""

pre_lit_1 = """
#######################################
# Constants (LITERAL1)
#######################################

"""

@click.command()
@click.argument('files', type=click.Path("r"), nargs=-1)

def build(files):
    literal_1_data = ""
    keyword_1_data = ""
    keyword_2_data = ""

    for file in files:
        with open(file, "r") as a_file:
            for line in a_file:
                stripped_line = line.strip()

                if len(stripped_line) > 9 and not stripped_line.startswith(tuple(prefixes)) and not stripped_line.endswith(tuple(sufixes)):
                    if re.search(r"^.+\s[a-z][A-Za-z]+\(.*", stripped_line) != None:
                        words = re.findall(r"\w+", stripped_line)
                        methodName = words[1]
                        if words[0] == 'const':
                            methodName = words[2]
                        keyword_2_data += methodName + "	KEYWORD2\n"
                if len(stripped_line) > 7 and stripped_line.startswith('class '):
                    if re.search(r"\w+", stripped_line) != None:
                        methodName = re.findall(r"\w+", stripped_line)[1]
                        keyword_1_data += methodName + "	KEYWORD1\n"
                if len(stripped_line) > 7 and stripped_line.startswith('#ifndef') and not stripped_line.endswith('_H'):
                    if re.search(r"\w+", stripped_line) != None:
                        methodName = re.findall(r"\w+", stripped_line)[1]
                        if re.search(methodName, literal_1_data) == None:
                            literal_1_data += methodName + "	LITERAL1\n"

    with open("keywords.txt", "w") as a_file:
        a_file.truncate()
        a_file.write(pre_key_1)
        a_file.write(keyword_1_data)
        a_file.write(pre_key_2)
        a_file.write(keyword_2_data)
        a_file.write(pre_lit_1)
        a_file.write(literal_1_data)
        print("Done")
    
if __name__ == '__main__':
    build()