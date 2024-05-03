#pragma once
#include <Arduino.h>

#ifdef MenuItem_H
/**
 * Processes the given command on the LcdMenu object.
 *
 * @param menu The LcdMenu object on which the command is to be processed.
 * @param cmd The command to be processed.
 * @param upCmd The command for moving up in the menu.
 * @param downCmd The command for moving down in the menu.
 * @param enterCmd The command for entering/editing a value.
 * @param backCmd The command for going back in the menu.
 * @param leftCmd The command for moving left in the menu.
 * @param rightCmd The command for moving right in the menu.
 *
 * @return true if the command is consumed, false otherwise.
 *
 * @throws None
 */
bool processCommand(LcdMenu& menu, byte cmd, byte upCmd, byte downCmd,
                    byte enterCmd, byte backCmd, byte leftCmd, byte rightCmd) {
    if (cmd == upCmd) {
        menu.up();
    } else if (cmd == downCmd) {
        menu.down();
    } else if (cmd == leftCmd) {
        menu.left();
    } else if (cmd == rightCmd) {
        menu.right();
    } else if (cmd == enterCmd) {
        menu.enter();
    } else if (cmd == backCmd) {
        menu.back();
    } else {
        return false;
    }
    return true;
}
#endif

#if defined(MenuItem_H) && !defined(ItemInput_H)
/**
 * @brief Process general menu navigation commands for the LCD menu.
 *
 * This function interprets the provided command and performs corresponding
 * actions on the given LCD menu for general navigation purposes.
 *
 * @param menu      The LcdMenu instance representing the LCD menu.
 * @param cmd       The byte value representing the input command to be
 *                  processed.
 * @param upCmd     The byte value representing the command for moving up in the
 *                  menu.
 * @param downCmd   The byte value representing the command for moving down in
 *                  the menu.
 * @param enterCmd  The byte value representing the command for entering a menu
 *                  item or mode.
 * @param backCmd   The byte value representing the command for going back in
 *                  the menu.
 * @param leftCmd   The optional byte value representing the command for moving
 *                  left in the menu.
 * @param rightCmd  The optional byte value representing the command for moving
 *                  right in the menu.
 * @return          `true` if the command is consumed, `false` otherwise.
 */
bool processMenuCommand(LcdMenu& menu, byte cmd, byte upCmd, byte downCmd,
                        byte enterCmd, byte backCmd, byte leftCmd = 255,
                        byte rightCmd = 255) {
    return processCommand(menu, cmd, upCmd, downCmd, enterCmd, backCmd, leftCmd,
                          rightCmd);
}
#endif

#ifdef ItemInput_H
/**
 * @brief Process input commands for menu with ItemInput using a charset.
 *
 * This function interprets the provided command and performs corresponding
 * actions on the given LCD menu in the context of an ItemInput type.
 *
 * @param menu         The LcdMenu instance representing the LCD menu.
 * @param cmd          The byte value representing the input command to be
 *                     processed.
 * @param pos          The reference to an int8_t variable storing the current
 *                     position in the charset when in edit mode.
 * @param charset      The pointer to a constant character array representing
 *                     the charset.
 * @param size         The size of the charset.
 * @param upCmd        The byte value representing the command for moving up in
 *                     the menu.
 * @param downCmd      The byte value representing the command for
 *                     moving down in the menu.
 * @param enterCmd     The byte value representing the command for
 *                     entering/editing a value.
 * @param backCmd      The byte value representing the command for going back in
 *                     the menu.
 * @param clearCmd     The optional byte value representing the command for
 *                     clearing the value.
 * @param backspaceCmd The optional byte value representing the command for
 *                     backspacing.
 * @param leftCmd      The optional byte value representing the command for
 *                     moving left in the menu.
 * @param rightCmd     The optional byte value representing the command for
 *                     moving right in the menu.
 * @return             `true` if the command is consumed, `false` otherwise.
 */
bool processMenuCommand(LcdMenu& menu, byte cmd, int8_t& pos,
                        const char* charset, uint8_t size, byte upCmd,
                        byte downCmd, byte enterCmd, byte backCmd,
                        byte clearCmd = 255, byte backspaceCmd = 255,
                        byte leftCmd = 255, byte rightCmd = 255) {
    if (cmd == upCmd) {
        if (menu.isInEditMode()) pos = (pos + 1) % size;
        // Works only in edit mode
        menu.drawChar(charset[pos]);
        menu.up();
    } else if (cmd == downCmd) {
        if (menu.isInEditMode()) pos = constrain(pos - 1, 0, size);
        menu.drawChar(charset[pos]);
        menu.down();
    } else if (cmd == leftCmd) {
        menu.left();
    } else if (cmd == rightCmd) {
        menu.right();
    } else if (cmd == enterCmd) {
        if (pos != -1) menu.type(charset[pos]);
        pos = -1;
        menu.enter();
    } else if (cmd == backCmd) {
        pos = -1;
        menu.back();
    } else if (cmd == clearCmd) {
        pos = -1;
        menu.clear();
    } else if (cmd == backspaceCmd) {
        pos = -1;
        menu.backspace();
    } else {
        return false;
    }
    return true;
}

/**
 * @brief Process input commands for a menu with ItemInput.
 *
 * This function interprets the provided command and performs corresponding
 * actions on the given LCD menu in the context of an ItemInput type.
 *
 * @param menu         The LcdMenu instance representing the LCD menu.
 * @param cmd          The byte value representing the input command to be
 *                     processed.
 * @param upCmd        The byte value representing the command for moving up in
 *                     the menu.
 * @param downCmd      The byte value representing the command for
 *                     moving down in the menu.
 * @param leftCmd      The byte value representing the command for
 *                     moving left in the menu.
 * @param rightCmd     The byte value representing the command for
 *                     moving right in the menu.
 * @param enterCmd     The byte value representing the command for
 *                     entering/editing a value.
 * @param backCmd      The byte value representing the command for going back in
 *                     the menu.
 * @param clearCmd     The optional byte value representing the command for
 *                     clearing the value.
 * @param backspaceCmd The optional byte value representing the command for
 *                     backspacing.
 * @return             `true` if the command is consumed, `false` otherwise.
 */
bool processMenuCommand(LcdMenu& menu, byte cmd, byte upCmd, byte downCmd,
                        byte leftCmd, byte rightCmd, byte enterCmd,
                        byte backCmd, byte clearCmd = 255,
                        byte backspaceCmd = 255) {
    if (!processCommand(menu, cmd, upCmd, downCmd, enterCmd, backCmd, leftCmd,
                        rightCmd)) {
        if (cmd == clearCmd)
            menu.clear();
        else if (cmd == backspaceCmd)  // Remove one character from the tail
            menu.backspace();
        else if (cmd != '\n')  // Type the character you want
            menu.type(cmd);
        else
            return false;
    }
    return true;
}
#endif