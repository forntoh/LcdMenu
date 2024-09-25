import os

src_dir = 'src'
dest_dir = 'docs/source/reference/api'
skip_files = []
skip_dirs = ['utils']

def create_rst_file(src_path, dest_path):
    os.makedirs(os.path.dirname(dest_path), exist_ok=True)
    filename = os.path.basename(src_path).replace('.h', '')
    with open(dest_path, 'w') as rst_file:
        rst_file.write(f'{filename}\n')
        rst_file.write(f'{"=" * len(filename)}\n\n')
        rst_file.write(f'.. doxygenfile:: {filename}.h\n')
        rst_file.write(f'   :project: LcdMenu\n')
        
def create_index_file(directory, entries, subdirs, is_root=False):
    dir_name = os.path.basename(directory) if not is_root else "Class reference"
    index_path = os.path.join(directory, 'index.rst')
    with open(index_path, 'w') as index_file:
        index_file.write(f'{dir_name}\n')
        index_file.write(f'{"=" * len(dir_name)}\n\n')
        index_file.write('.. toctree::\n')
        index_file.write('   :maxdepth: 2\n')
        index_file.write(f'   :hidden:\n')
        index_file.write(f'   :caption: {dir_name}\n\n')
        for entry in entries:
            relative_entry = os.path.relpath(entry, directory).replace('.rst', '')
            index_file.write(f'   {relative_entry}\n')
        for subdir in subdirs:
            relative_subdir = os.path.basename(subdir)
            index_file.write(f'   {relative_subdir}/index\n')

def process_directory(directory, skip_files, skip_dirs, is_root=False):
    for root, dirs, files in os.walk(directory):
        dirs[:] = [d for d in dirs if d not in skip_dirs]

        index_entries = []
        subdirs = [os.path.join(root, d) for d in dirs]
        for file in files:
            if file.endswith('.h') and file not in skip_files:
                src_path = os.path.relpath(os.path.join(root, file), src_dir)
                dest_path = os.path.join(dest_dir, src_path).replace('.h', '.rst')
                create_rst_file(src_path, dest_path)
                index_entries.append(dest_path)
        
        if index_entries or subdirs:
            create_index_file(os.path.join(dest_dir, os.path.relpath(root, src_dir)), index_entries, subdirs, is_root)
            is_root = False

if __name__ == '__main__':
    process_directory(src_dir, skip_files, skip_dirs, is_root=True)