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

The generated HTML files will be available in the `docs/build/html` directory.

---

There are other documentation files that are generated using some custom scripts, run the `pre_build.bat` file to generate them:

- You will need to have Doxygen installed to generate the Doxygen documentation.

> sudo apt-get install -y doxygen

To run the pre_build.bat file in the terminal, you need to ensure it has execute permissions and then execute it. Here are the steps:

1. Give execute permissions to the script:

```bash
chmod +x pre_build.bat
```

2. Run the script:

```bash
docs/pre_build.bat
```
