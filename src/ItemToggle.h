#ifndef ItemToggle_H
#define ItemToggle_H

#include "LcdMenu.h"
#include "MenuItem.h"
#include <functional>

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
    std::function<void(bool)> callback = NULL;

  public:
    /**
     * @brief Create an ItemToggle object with default values as `ON` and `OFF`.
     *
     * @param key key of the item
     * @param callback reference to callback function
     */
    ItemToggle(const char* key, std::function<void(bool)> callback)
        : ItemToggle(key, false, callback) {}

    /**
     * @brief Construct a new Item Toggle object with an initial state
     * and default ON/OFF texts.
     *
     * @param text
     * @param enabled
     * @param callback
     */
    ItemToggle(const char* text, boolean enabled, std::function<void(bool)> callback)
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
    ItemToggle(const char* text, const char* textOn, const char* textOff, std::function<void(bool)> callback)
        : MenuItem(text),
          textOn(textOn),
          textOff(textOff),
          callback(callback) {}

    /**
     * @brief Get the integer callback function of this item.
     * @return the integer callback function
     */
    std::function<void(bool)> getCallbackInt() { return callback; }

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

/**
 * @brief Create a new toggle item.
 *
 * @param text The text to display for the item.
 * @param textOn The display text when ON.
 * @param textOff The display text when OFF.
 * @param callback The function to call when the item is toggled.
 * @return MenuItem* The created item. Caller takes ownership of the returned pointer.
 *
 * @example
 *   auto item = ITEM_TOGGLE("Toggle", "ON", "OFF", [](bool enabled) { Serial.println(enabled); });
 */
inline MenuItem* ITEM_TOGGLE(
    const char* text,
    const char* textOn,
    const char* textOff,
    std::function<void(bool)> callback) {
    return new ItemToggle(text, textOn, textOff, callback);
}

/**
 * @brief Create a new toggle item with default ON/OFF texts.
 *
 * @param text The text to display for the item.
 * @param enabled The initial state of the toggle.
 * @param callback The function to call when the item is toggled.
 * @return MenuItem* The created item. Caller takes ownership of the returned pointer.
 *
 * @example
 *   auto item = ITEM_TOGGLE("Toggle", true, [](bool enabled) { Serial.println(enabled); });
 */
inline MenuItem* ITEM_TOGGLE(
    const char* text,
    boolean enabled,
    std::function<void(bool)> callback) {
    return new ItemToggle(text, enabled, callback);
}

/**
 * @brief Create a new toggle item with default OFF state.
 *
 * @param text The text to display for the item.
 * @param callback The function to call when the item is toggled.
 * @return MenuItem* The created item. Caller takes ownership of the returned pointer.
 *
 * @example
 *   auto item = ITEM_TOGGLE("Toggle", [](bool enabled) { Serial.println(enabled); });
 */
inline MenuItem* ITEM_TOGGLE(
    const char* text,
    std::function<void(bool)> callback) {
    return new ItemToggle(text, callback);
}

#endif
