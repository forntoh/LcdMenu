# Writing Functionality Tests for LcdMenu

This guide provides instructions for you on how to write functionality tests for LcdMenu. Each YAML file corresponds to an example sketch in the [`examples`](/examples) directory.

## Test File Structure

Each test file should follow a similar structure:

```yaml
version: 1
author: Your Name
steps:
  - wait-serial: "Expected Serial Output"
  - simulate: button-action
  - wait-serial: "Expected Serial Output"
  - simulate: button-action
  ...
```

### Key Elements

- version: The version of the test file format.
- author: The author of the test file.
- steps: A sequence of actions and expected outputs.

### Actions

- wait-serial: Waits for a specific serial output.
- simulate: Simulates a button press or other input.
  - There are currently 9 supported actions:
    - `upButton-press`: Simulates pressing the up button.
    - `downButton-press`: Simulates pressing the down button.
    - `leftButton-press`: Simulates pressing the left button.
    - `rightButton-press`: Simulates pressing the right button.
    - `enterButton-press`: Simulates pressing the enter button.
    - `backButton-press`: Simulates pressing the back button.
    - `backspaceButton-press`: Simulates pressing the backspace button.
    - `*-down`: Presses a button without releasing it (used for hold tests).
    - `*-up`: Releases a previously held button.

## Example Test Files

Each YAML file in this directory matches an example sketch in the [`examples`](/examples) directory. For instance, `CharsetInput.test.yml` corresponds to [`examples/CharsetInput/CharsetInput.ino`](/examples/CharsetInput/CharsetInput.ino)

### Steps to Create a Test File

1. **Identify the Example Sketch**: Determine which example sketch the test file will correspond to.
1. **Define the Test Steps**: Outline the sequence of actions and expected outputs.
1. **Write the YAML File**: Follow the structure and key elements outlined above to create the test file.
1. **Save the File**: Ensure the test file is saved in the same directory as this guide.

> **Hint**: You can run the tests locally using `act` to verify that they work as expected.
> You can install `act` using
>
> - `brew install act` on macOS
> - `sudo apt install act` on Linux.
> - `choco install act-cli` on Windows
>
> You should also have Docker installed on your machine.
> To run the tests, use the command
>
> ```bash
> act -W .github/workflows/wokwi_ci.yml pull_request
> ```

To run tests that interact with the Wokwi simulator you must set the
`WOKWI_CLI_TOKEN` environment variable to your personal token. You can obtain
this token from the [Wokwi dashboard](https://wokwi.com/dashboard/ci).

The workflow calculates the required timeout for each test using
`.scripts/prepare_workflow.py` before launching `wokwi-cli`.

By following these guidelines, you can create consistent and effective functionality tests for your feature/enhancement.
