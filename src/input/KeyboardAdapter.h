#pragma once

#include "InputInterface.h"
#include "Stream.h"

#define BS 8     // Backspace, \b
#define LF 10    // Line feed, \n
#define CR 13    // Carriage Return, \r
#define ESC 27   // Escape
#define DEL 127  // Del, Backspace on Mac
#define C2_CSI_TERMINAL_MIN 0x40
#define C2_CSI_TERMINAL_MAX 0x7E
#define THRESHOLD 100
#define CSI_BUFFER_SIZE 5

/**
 * @brief Input interface to keyboard.
 * Uses default buttons layout:
 * - `Arrows` for `LEFT/RIGHT/UP/DOWN`;
 * - `Enter` for `ENTER`;
 * - `Escape` for `BACK`;
 * - `Delete` for `CLEAR`;
 * - `Backspace` for `BACKSPACE`;
 *
 * Keyboard can send multiple-bytes commands.
 * Implementation should convert it to one byte command.
 *
 * Implementation details. Mapping:
 * `First 128 of ASCII`        -> as is
 * `\r`                        -> `ENTER`
 * `\n`                        -> `ENTER`
 * `\r\n`                      -> `ENTER`
 * `ESC`                       -> `BACK`
 * `ESC [ A` (up arrow)        -> `UP`
 * `ESC [ B` (down arrow)      -> `DOWN`
 * `ESC [ C` (right arrow)     -> `RIGHT`
 * `ESC [ D` (left arrow)      -> `LEFT`
 * `ESC [ 3 ~` (Delete button) -> `CLEAR`
 */
class KeyboardAdapter : public InputInterface {
  private:
    /**
     * @see https://en.wikipedia.org/wiki/ANSI_escape_code
     * @see https://gist.github.com/fnky/458719343aabd01cfb17a3a4f7296797
     */
    enum class CodeSet {
        /**
         * @brief Main ASCII code set.
         * Initially defined as part of ASCII, the default C0 control code set is now defined in ISO 6429 (ECMA-48).
         */
        C0,
        /**
         * @brief Fe Escape sequences.
         * Starts with ESC.
         * Terminates with [0x40, 0x5F].
         */
        C1,
        /**
         * @brief Control Sequence Introducer.
         * Starts with ESC [.
         * Terminates with [0x40, 0x7E].
         */
        C2_CSI,
        /**
         * @brief Device Control String.
         * Starts with ESC P.
         * Terminates with ESC \.
         */
        C2_DCS,
        /**
         * @brief Operating System Command.
         * Starts with ESC ].
         * Terminates with ESC \.
         * Currently not implemented.
         */
        C2_OSC,
        /**
         * @brief Expect string terminator.
         * Currently not implemented.
         */
        C3,
    };
    /**
     * @brief Input stream.
     */
    Stream* stream = NULL;
    /**
     * @brief Internal state of current code set.
     * As stream receives bytes asynchronously, multiple bytes command can arrive
     * in several calls. Need to store current state between calls.
     */
    CodeSet codeSet = CodeSet::C0;
    /**
     * @brief Last received character.
     * Used to detect 2 chars sequences, e.g. `\r\n`.
     */
    unsigned char lastChar;
    /**
     * @brief Milliseconds timestamp of last received character.
     * Used for detecting ESC with no chars next or single `\r` without `\n`.
     */
    unsigned long lastCharTimestamp;
    /**
     * @brief Buffer to store already read values of "intermediate bytes".
     * Multiple bytes command for CSI is in form of `ESC [ <intermediate bytes> <terminal byte>`.
     */
    char csiBuffer[CSI_BUFFER_SIZE];
    /**
     * @brief Points to next available byte in `csiBuffer`.
     */
    uint8_t csiBufferCursor = 0;
    /**
     * @brief Reset to initial state.
     */
    inline void reset() {
        codeSet = CodeSet::C0;
        csiBufferCursor = 0;
        lastChar = 0;
        lastCharTimestamp = 0;
    }
    inline bool hasLastChar() {
        // TODO: Check millis() overflow situation
        return lastChar != 0 && millis() > lastCharTimestamp + THRESHOLD;
    }
    inline void saveLastChar(unsigned char command) {
        lastChar = command;
        lastCharTimestamp = millis();
    }
    /**
     * @brief Handle idle state when there are no input for some time.
     * @see THRESHOLD - timeout in ms.
     */
    void handleIdle();
    /**
     * @brief Handle received command.
     * @param command the received command
     */
    void handleReceived(unsigned char command);

  public:
    KeyboardAdapter(LcdMenu* menu, Stream* stream)
        : InputInterface(menu), stream(stream) {
    }
    void observe() override {
        if (!stream->available()) {
            if (hasLastChar()) {
                handleIdle();
                reset();
            }
            return;
        }
        unsigned char command = stream->read();
        printCmd(F("INPUT"), (uint8_t)command);
        handleReceived(command);
    }
};
