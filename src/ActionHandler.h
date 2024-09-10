#ifndef ACTION_HANDLER_H
#define ACTION_HANDLER_H

class ActionHandler {
   public:
    /**
     * Execute an "up press" on menu
     * When edit mode is enabled, this action is skipped
     * @return true if the action was successfully handled and not skipped
     */
    virtual bool up() { return false; };
    /**
     * Execute a "down press" on menu
     * When edit mode is enabled, this action is skipped
     * @return true if the action was successfully handled and not skipped
     */
    virtual bool down() { return false; };
    /**
     * Execute an "enter" action on menu.
     *
     * It does the following depending on the type of the current menu item:
     *
     * - Open a sub menu.
     * - Execute a callback action.
     * - Toggle the state of an item.
     * @return true if the action was successfully handled and not skipped
     */
    virtual bool enter() { return false; };
    /**
     * Execute a "backpress" action on menu.
     *
     * Navigates up once.
     * @return true if the action was successfully handled and not skipped
     */
    virtual bool back() { return false; };
    /**
     * Execute a "left press" on menu
     *
     * *NB: Works only for `ItemInput` and `ItemList` types*
     *
     * Moves the cursor one step to the left.
     * @return true if the action was successfully handled and not skipped
     */
    virtual bool left() { return false; };
    /**
     * Execute a "right press" on menu
     *
     * *NB: Works only for `ItemInput` and `ItemList` types*
     *
     * Moves the cursor one step to the right.
     * @return true if the action was successfully handled and not skipped
     */
    virtual bool right() { return false; };
    /**
     * Execute a "backspace cmd" on menu
     *
     * *NB: Works only for `ItemInput` type*
     *
     * Removes the character at the current cursor position.
     * @return true if the action was successfully handled and not skipped
     */
    virtual bool backspace() { return false; };
    /**
     * Display text at the cursor position
     * used for `Input` type menu items
     * @param c character to append
     * @return true if the action was successfully handled and not skipped
     */
    virtual bool type(const char c) { return false; };
    /**
     * Clear the value of the input field.
     * @return true if the action was successfully handled and not skipped
     */
    virtual bool clear() { return false; };
};

#endif
