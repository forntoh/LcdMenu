#pragma once

#include "InputInterface.h"
#include "Stream.h"

#define ESC 27  // Escape
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
     * Input stream.
     */
    Stream* stream = NULL;
    /**
     * Internal state of current code set.
     * As stream receives bytes asynchronously, multiple bytes command can arrive
     * in several calls. Need to store current state between calls.
     */
    CodeSet codeSet = CodeSet::C0;
    /**
     * Milliseconds timestamp of last received character.
     * Used for detecting ESC with no chars next.
     */
    unsigned long lastCharTimestamp;
    /**
     * @brief Buffer to store already read values of "intermediate bytes".
     * Multiple bytes command for CSI is in form of `ESC [ <intermediate bytes> <terminal byte>`.
     */
    char csiBuffer[CSI_BUFFER_SIZE];
    /**
     * Points to available byte in `buffer`.
     */
    uint8_t csiBufferCursor = 0;
    /**
     * Reset to initial state.
     */
    void reset();

  public:
    KeyboardAdapter(LcdMenu* menu, Stream* stream)
        : InputInterface(menu), stream(stream) {
    }
    void loop() override;
};
