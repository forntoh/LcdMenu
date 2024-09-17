/**
 * ---
 *
 * # ItemToggle
 *
 * This item type indicates that the current item is **toggleable**.
 * When `enter()` is invoked, the state of `isOn` is toggled.
 */

#ifndef ItemToggle_H
#define ItemToggle_H
#include "MenuItem.h"
#include <utils/utils.h>

/**
 * @brief Item that allows user to toggle between ON/OFF states.
 *
 * ┌────────────────────────────┐
 * │ > T E X T : O F F          │
 * └────────────────────────────┘
 *
 * Additionally to `text` this item has ON/OFF `enabled` state.
 */
class ItemToggle : public MenuItem {
  private:
    bool enabled = false;
    const char* textOn = NULL;
    const char* textOff = NULL;
    fptrInt callback = NULL;

  public:
    /**
     * @param key key of the item
     * @param callback reference to callback function
     */
    ItemToggle(const char* key, fptrInt callback)
        : ItemToggle(key, false, callback) {}

    ItemToggle(const char* text, boolean enabled, fptrInt callback)
        : ItemToggle(text, "ON", "OFF", callback) {
        this->enabled = enabled;
    }

    /**
     * @brief Create an ItemToggle object.
     * @param key key of the item
     * @param textOn display text when ON
     * @param textOff display text when OFF
     * @param callback reference to callback function
     */
    ItemToggle(const char* key, const char* textOn, const char* textOff, fptrInt callback)
        : MenuItem(key, MENU_ITEM_TOGGLE),
          textOn(textOn),
          textOff(textOff),
          callback(callback) {}

    /**
     * @brief Get the integer callback function of this item.
     * @return the integer callback function
     */
    fptrInt getCallbackInt() { return callback; }

    /**
     * @brief Get the current state of this toggle item.
     * @return the current state
     */
    boolean isOn() { return enabled; }

    /**
     * @brief Set the current state of this toggle item.
     * @param isOn the new state
     */
    void setIsOn(boolean isOn) { this->enabled = isOn; }

    const char* getTextOn() { return this->textOn; }

    const char* getTextOff() { return this->textOff; }

    void draw(uint8_t row) override {
        uint8_t maxCols = display->getMaxCols();
        static char* buf = new char[maxCols];
        substring(enabled ? textOn : textOff, 0, maxCols - strlen(text) - 2, buf);
        display->drawItem(row, text, ':', buf);
    };

    bool process(const unsigned char c) override {
        switch (c) {
            case ENTER: return enter();
            default: return false;
        }
    };

  protected:
    bool enter() {
        enabled = !enabled;
        if (callback != NULL) {
            callback(enabled);
        }
        MenuItem::draw();
        return true;
    };
};

#define ITEM_TOGGLE(...) (new ItemToggle(__VA_ARGS__))

#endif
