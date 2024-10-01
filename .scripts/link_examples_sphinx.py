import os

src_dir = 'examples'
dest_dir = 'docs/source/reference/samples'
skip_files = []
skip_dirs = []

def create_rst_file(src_path, dest_path):
    os.makedirs(os.path.dirname(dest_path), exist_ok=True)
    filename = os.path.basename(src_path).replace('.ino', '')
    with open(dest_path, 'w') as rst_file:
        rst_file.write(f'{filename}\n')
        rst_file.write(f'{"=" * len(filename)}\n\n')
        rst_file.write(f'.. literalinclude:: ../../../../{src_dir}/{src_path}\n')
        rst_file.write(f'   :language: cpp\n')
        rst_file.write(f'   :linenos:\n\n')

def create_index_file(directory, entries):
    dir_name = "Code samples"
    index_path = os.path.join(directory, 'index.rst')
    os.makedirs(os.path.dirname(index_path), exist_ok=True)
    with open(index_path, 'w') as index_file:
        index_file.write(f'{dir_name}\n')
        index_file.write(f'{"=" * len(dir_name)}\n\n')
        index_file.write('.. rst-class:: lead\n\n')
        index_file.write('This section contains code samples that demonstrate how to use the |project| library.\n')
        index_file.write('Each sample is a complete program that shows how to use the library to accomplish a specific task.\n')
        index_file.write('You can find the source code for each sample in the `examples <https://github.com/forntoh/LcdMenu/tree/master/examples>`_ directory of the library repository.\n\n')
        index_file.write('-'*80)
        index_file.write('\n\n')
        index_file.write('.. toctree::\n')
        index_file.write('   :maxdepth: 1\n')
        index_file.write(f'   :caption: {dir_name}\n\n')
        for entry in entries:
            print(entry)
            relative_entry = os.path.relpath(entry, directory).replace('.rst', '')
            index_file.write(f'   {relative_entry}\n')

def process_directory(directory, skip_files, skip_dirs):
    index_entries = []

    for root, dirs, files in os.walk(directory):
        dirs[:] = [d for d in dirs if d not in skip_dirs]

        for file in files:
            if file.endswith('.ino') and file not in skip_files:
                src_path = os.path.relpath(os.path.join(root, file), src_dir)
                dest_path = os.path.join(dest_dir, os.path.basename(file)).replace('.ino', '.rst')
                create_rst_file(src_path, dest_path)
                print(f'Created {dest_path}')
                index_entries.append(dest_path)
        
    if index_entries:
        create_index_file(dest_dir, index_entries)

if __name__ == '__main__':
    process_directory(src_dir, skip_files, skip_dirs)