#include "KeyboardAdapter.h"

void KeyboardAdapter::reset() {
    codeSet = CodeSet::C0;
    csiBufferCursor = 0;
}

void KeyboardAdapter::loop() {
    if (!stream->available()) {
        if (millis() > lastCharTimestamp + THRESHOLD) {
            if (codeSet == CodeSet::C1) {
                // When ESC but no further chars
                printCmd(F("Call BACK")); 
                menu->handle(BACK); 
            }
            if (codeSet != CodeSet::C0) {
                reset();
            }
        }
        return;
    }
    char command = stream->read();
    lastCharTimestamp = millis();
    printCmd(F("INPUT"), (uint8_t)command);
    switch (codeSet) {
        case CodeSet::C0:
            switch (command) {
                case ESC: codeSet = CodeSet::C1; return;
                default: printCmd(F("Call"), command); menu->handle(command); return;
            }
        case CodeSet::C1:
            switch (command) {
                case '[': codeSet = CodeSet::C2_CSI; return;
                default: reset(); return;
            }
        case CodeSet::C2_CSI:
            if (command >= C2_CSI_TERMINAL_MIN && command <= C2_CSI_TERMINAL_MAX) {
                switch (command) {
                    case 'A': printCmd(F("Call UP")); menu->handle(UP); reset(); return;
                    case 'B': printCmd(F("Call DOWN")); menu->handle(DOWN); reset(); return;
                    case 'C': printCmd(F("Call RIGHT")); menu->handle(RIGHT); reset(); return;
                    case 'D': printCmd(F("Call LEFT")); menu->handle(LEFT); reset(); return;
                    case '~':
                        if (csiBufferCursor > 0) {
                            switch (csiBuffer[0] - '0') {
                                case 2: printCmd(F("Insert")); reset(); return; // Insert
                                case 3: printCmd(F("Call CLEAR")); menu->handle(CLEAR); reset(); return;
                                case 5: printCmd(F("PgUp")); reset(); return; // PgUp
                                case 6: printCmd(F("PgDn")); reset(); return; // PgDn
                                default: reset(); return;
                            }
                        }
                    default: reset(); return;
                }
            } else {
                csiBuffer[csiBufferCursor] = command;
                csiBufferCursor++;
                return;
            }
        default: reset(); return;
    }
}
