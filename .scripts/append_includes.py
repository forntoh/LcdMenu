import os
import re
import sys


def replace_includes(directory, sort_order='asc', use_toctree=False):
    include_pattern = re.compile(r'\.\. include::\s+\S+')
    toctree_pattern = re.compile(r'\.\. toctree::\s+.*?(\n\s+.*?)*', re.DOTALL)
    maxdepth_pattern = re.compile(r':maxdepth:\s+\d+')
    rst_file_pattern = re.compile(r'^\s+\S+\.rst$', re.MULTILINE)
    
    for root, _, files in os.walk(directory):
        if 'index.rst' in files:
            index_path = os.path.join(root, 'index.rst')
            print(f"Processing {index_path}")
            with open(index_path, 'r') as index_file:
                content = index_file.read()
            
            # Remove existing include, toctree, maxdepth, and rst file directives
            content = include_pattern.sub('', content)
            content = toctree_pattern.sub('', content)
            content = maxdepth_pattern.sub('', content)
            content = rst_file_pattern.sub('', content).strip()
            
            rst_files = [file for file in files if file.endswith('.rst') and file != 'index.rst']
            rst_files.sort(reverse=(sort_order == 'desc'))
            
            if use_toctree:
                content += '\n\n.. toctree::\n   :maxdepth: 2\n\n'
                for file in rst_files:
                    content += f'   {file}\n'
                    print(f"Added {file} to toctree")
            else:
                content += '\n'
                for file in rst_files:
                    content += f'\n.. include:: {file}'
                    print(f"Added include directive for {file}")
            
            with open(index_path, 'w') as index_file:
                index_file.write(content.strip() + '\n')
            print(f"Updated {index_path}")

if __name__ == "__main__":
    if len(sys.argv) < 2 or len(sys.argv) > 4:
        print("Usage: python append_includes.py <directory_path> [sort_order] [use_toctree]")
        sys.exit(1)
    
    directory_path = sys.argv[1]
    sort_order = sys.argv[2] if len(sys.argv) >= 3 else 'asc'
    use_toctree = sys.argv[3].lower() == 'true' if len(sys.argv) == 4 else False
    
    if sort_order not in ['asc', 'desc']:
        print("Invalid sort order. Use 'asc' for ascending or 'desc' for descending.")
        sys.exit(1)
    
    replace_includes(directory_path, sort_order, use_toctree)