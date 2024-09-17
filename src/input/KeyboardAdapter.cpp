#define DEBUG

#include "KeyboardAdapter.h"
#include "utils/utils.h"

void KeyboardAdapter::handleIdle() {
    switch (lastChar) {
        case CR:  // Received single `\r`
            printCmd(F("Call ENTER from idle"));
            menu->process(ENTER);
            break;
        case ESC:  // Received single `ESC`
            printCmd(F("Call BACK from idle"));
            menu->process(BACK);
            break;
    }
}
void KeyboardAdapter::handleReceived(unsigned char command) {
    switch (codeSet) {
        case CodeSet::C0:
            switch (command) {
                case BS:   // 8. On Win
                case DEL:  // 127. On Mac
                    printCmd(F("Call BACKSPACE"));
                    menu->process(BACKSPACE);
                    break;
                case LF:  // 10, \n
                    printCmd(F("Call ENTER"));
                    menu->process(ENTER);
                    break;
                case CR:  // 13, \r
                    // Can be \r\n sequence, do nothing
                    break;
                case ESC:  // 27
                    codeSet = CodeSet::C1;
                    break;
                default:
                    printCmd(F("Call default"), (uint8_t)command);
                    menu->process(command);
                    break;
            }
            saveLastChar(command);
            break;
        case CodeSet::C1:
            switch (command) {
                case '[':
                    codeSet = CodeSet::C2_CSI;
                    break;
                default:
                    reset(); // Reset after unsupported C1 commmand
                    break;
            }
            break;
        case CodeSet::C2_CSI:
            if (command >= C2_CSI_TERMINAL_MIN && command <= C2_CSI_TERMINAL_MAX) {
                switch (command) {
                    case 'A':
                        printCmd(F("Call UP"));
                        menu->process(UP);
                        break;
                    case 'B':
                        printCmd(F("Call DOWN"));
                        menu->process(DOWN);
                        break;
                    case 'C':
                        printCmd(F("Call RIGHT"));
                        menu->process(RIGHT);
                        break;
                    case 'D':
                        printCmd(F("Call LEFT"));
                        menu->process(LEFT);
                        break;
                    case 'F':
                        printCmd(F("End"));
                        break;
                    case 'H':
                        printCmd(F("Home"));
                        break;
                    case '~':
                        if (csiBufferCursor > 0) {
                            switch (csiBuffer[0] - '0') {
                                case 2:  // Insert
                                    printCmd(F("Insert"));
                                    break;
                                case 3:  // Delete
                                    printCmd(F("Call CLEAR"));
                                    menu->process(CLEAR);
                                    break;
                                case 5:  // PgUp
                                    printCmd(F("PgUp"));
                                    break;
                                case 6:  // PgDn
                                    printCmd(F("PgDn"));
                                    break;
                            }
                        }
                }
                reset(); // Reset after C2 terminal symbol
            } else {
                csiBuffer[csiBufferCursor] = command;
                csiBufferCursor++;
            }
            break;
        default:
            reset(); // Reset after unknown code set
            break;
    }
}
