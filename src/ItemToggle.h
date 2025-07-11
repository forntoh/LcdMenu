#ifndef ItemToggle_H
#define ItemToggle_H

#include "LcdMenu.h"
#include "MenuItem.h"
#include <utils/utils.h>

/**
 * @brief Item that allows user to toggle between ON/OFF states.
 *
 * ```
 * ┌────────────────────────────┐
 * │ > T E X T : O F F          │
 * └────────────────────────────┘
 * ```
 *
 * Additionally to `text` this item has ON/OFF `enabled` state.
 */
class ItemToggle : public MenuItem {
  private:
    bool enabled = false;
    const char* textOn = NULL;
    const char* textOff = NULL;
    fptrBool callback = NULL;

  public:
    /**
     * @brief Create an ItemToggle object with default values as `ON` and `OFF`.
     *
     * @param key key of the item
     * @param callback reference to callback function
     */
    ItemToggle(const char* key, fptrBool callback)
        : ItemToggle(key, false, callback) {}

    /**
     * @brief Construct a new Item Toggle object with an initial state
     * and default ON/OFF texts.
     *
     * @param text
     * @param enabled
     * @param callback
     */
    ItemToggle(const char* text, boolean enabled, fptrBool callback)
        : ItemToggle(text, "ON", "OFF", callback) {
        this->enabled = enabled;
    }

    /**
     * @brief Construct a new Item Toggle object.
     * @param text the text of the item
     * @param textOn display text when ON
     * @param textOff display text when OFF
     * @param callback reference to callback function
     */
    ItemToggle(const char* text, const char* textOn, const char* textOff, fptrBool callback)
        : MenuItem(text),
          textOn(textOn),
          textOff(textOff),
          callback(callback) {}

    /**
     * @brief Get the integer callback function of this item.
     * @return the integer callback function
     */
    fptrBool getCallbackInt() { return callback; }

    /**
     * @brief Get the current state of this toggle item.
     * @return the current state
     */
    boolean isOn() { return enabled; }

    /**
     * @brief Set the current state of this toggle item.
     * @note You need to call `LcdMenu::refresh` after this method to see the changes.
     * @param isOn the new state
     */
    void setIsOn(boolean isOn) { this->enabled = isOn; }

    const char* getTextOn() { return this->textOn; }

    const char* getTextOff() { return this->textOff; }

    uint8_t measureValueWidth(GraphicalDisplayInterface* display) override {
        if (!display) return 0;
        uint8_t wOn = display->getTextWidth(textOn ? textOn : "");
        uint8_t wOff = display->getTextWidth(textOff ? textOff : "");
        return wOn > wOff ? wOn : wOff;
    }

    void draw(MenuRenderer* renderer) override {
        renderer->drawItem(text, enabled ? textOn : textOff);
    };

  protected:
    bool process(LcdMenu* menu, const unsigned char command) override {
        MenuRenderer* display = menu->getRenderer();
        switch (command) {
            case ENTER:
                toggle(display);
                return true;
            default:
                return false;
        }
    };
    void toggle(MenuRenderer* renderer) {
        enabled = !enabled;
        LOG(F("ItemToggle::toggle"), enabled ? textOn : textOff);
        draw(renderer);
        if (callback != NULL) {
            callback(enabled);
        }
    }
};

#define ITEM_TOGGLE(...) (new ItemToggle(__VA_ARGS__))

#endif
