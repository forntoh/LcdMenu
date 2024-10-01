# How to build the documentation locally

To build the documentation locally, you need to install the required dependencies and run the build command.

## Install dependencies

To install the required dependencies, run the following command:

```bash
pip install -r docs/requirements.txt
```

## Build the documentation

To build the documentation, run the following command:

```bash
make -C docs html
```

There are other documentation files that are generated using some custom scripts. To generate these files, run the following commands:

You will need to have Doxygen installed to generate the Doxygen documentation.

```bash
# Generate Doxygen documentation
doxygen docs/Doxyfile

# Generate Sphinx documentation from Doxygen
python .scripts/link_doxygen_sphinx.py

# Link Examples
python .scripts/link_examples_sphinx.py

# Run changelog updater
python .scripts/update_changelog.py

# Append includes to index files
python .scripts/append_includes.py docs/source/reference/migration desc false
```

The generated HTML files will be available in the `docs/build/html` directory.
