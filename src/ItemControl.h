#ifndef ItemControl_H
#define ItemControl_H

#include "Control.h"
#include "LcdMenu.h"
#include "MenuItem.h"
#include "printf.h"
#include <utils/utils.h>

template <typename T0, typename T1 = void, typename T2 = void>
class ItemControl : public MenuItem {
  protected:
    Control<T0>* control0;
    Control<T1>* control1;
    Control<T2>* control2;
    void (*callback)(T0, T1, T2);
    uint8_t current = 0;
    const uint8_t getControlsCount() {
        return 3;
    }

  public:
    /**
     * @brief Construct a new Item Range Base object
     *
     * @param text The text of the item.
     * @param control0 The first control.
     * @param control1 The second control.
     * @param control2 The third control.
     * @param callback A pointer to the callback function to execute when this menu item is selected.
     */
    ItemControl(
        const char* text,
        Control<T0>* control0,
        Control<T1>* control1,
        Control<T2>* control2,
        void (*callback)(T0, T1, T2))
        : MenuItem(text), control0(control0), control1(control1), control2(control2), callback(callback) {}
    void setValues(T0 value0, T1 value1, T2 value2) {
        control0->setValue(value0);
        control1->setValue(value1);
        control2->setValue(value2);
    }

  protected:
    void draw(DisplayInterface* display, uint8_t row) override {
        uint8_t maxCols = display->getMaxCols();
        uint8_t col = 1;
        static uint8_t cols[3];
        static char* buf = new char[maxCols];
        display->drawItem(row, text, ':', buf);
        col += strlen(text) + 1;
        col += control0->draw(display, row, col);
        cols[0] = col;
        col += control1->draw(display, row, col);
        cols[1] = col;
        col += control2->draw(display, row, col);
        cols[2] = col;
        display->clearAfter(row, col);
        if (display->getEditModeEnabled()) {
            display->drawBlinker();
            // Serial.print("resetBlinker (col=");
            // Serial.print(cols[current], 10);
            // Serial.println(") ");
            uint8_t blinkerOffset = 0;
            switch (current) {
                case 0: blinkerOffset = control0->getBlinkerOffset(); break;
                case 1: blinkerOffset = control1->getBlinkerOffset(); break;
                case 2: blinkerOffset = control2->getBlinkerOffset(); break;
            }
            display->resetBlinker(cols[current] - 1 - blinkerOffset);
        }
    }

    bool process(LcdMenu* menu, const unsigned char command) override {
        DisplayInterface* display = menu->getDisplay();
        bool controlRes = false;
        switch (current) {
            case 0: controlRes = control0->process(menu, command); break;
            case 1: controlRes = control1->process(menu, command); break;
            case 2: controlRes = control2->process(menu, command); break;
        }
        if (controlRes) {
            MenuItem::draw(display);
            return true;
        }
        if (display->getEditModeEnabled()) {
            switch (command) {
                case RIGHT:
                case ENTER:
                    if (getControlsCount() <= 1) {
                        return false;
                    }
                    current++;
                    current %= getControlsCount();
                    MenuItem::draw(display);
                    return true;
                case LEFT:
                    if (getControlsCount() <= 1) {
                        return false;
                    }
                    current--;
                    if (current < 0) {
                        current = getControlsCount();
                    }
                    MenuItem::draw(display);
                    return true;
                case BACK:
                    display->setEditModeEnabled(false);
                    if (callback != NULL) {
                        callback(control0->getValue(), control1->getValue(), control2->getValue());
                    }
                    display->clearBlinker();
                    printLog(F("ItemControl::exitEditMode"), control0->getValue());
                    return true;
                default:
                    return false;
            }
        } else {
            switch (command) {
                case ENTER:
                    display->setEditModeEnabled(true);
                    printLog(F("ItemControl::enterEditMode"), control0->getValue());
                    MenuItem::draw(display);
                    return true;
                default:
                    return false;
            }
        }
    }
};

template <typename T0, typename T1>
class ItemControl<T0, T1, void> : public MenuItem {
  protected:
    Control<T0>* control0;
    Control<T1>* control1;
    void (*callback)(T0, T1);
    uint8_t current = 0;
    const uint8_t getControlsCount() {
        return 2;
    }

  public:
    ItemControl(
        const char* text,
        Control<T0>* control0,
        Control<T1>* control1,
        void (*callback)(T0, T1))
        : MenuItem(text), control0(control0), control1(control1), callback(callback) {}
    void setValues(T0 value0, T1 value1) {
        control0->setValue(value0);
        control1->setValue(value1);
    }

  protected:
    void draw(DisplayInterface* display, uint8_t row) override {
        uint8_t maxCols = display->getMaxCols();
        uint8_t col = 1;
        static uint8_t cols[3];
        static char* buf = new char[maxCols];
        display->drawItem(row, text, ':', buf);
        col += strlen(text) + 1;
        col += control0->draw(display, row, col);
        cols[0] = col;
        col += control1->draw(display, row, col);
        cols[1] = col;
        display->clearAfter(row, col);
        if (display->getEditModeEnabled()) {
            display->drawBlinker();
            uint8_t blinkerOffset = 0;
            switch (current) {
                case 0: blinkerOffset = control0->getBlinkerOffset(); break;
                case 1: blinkerOffset = control1->getBlinkerOffset(); break;
            }
            display->resetBlinker(cols[current] - 1 - blinkerOffset);
        }
    }

    bool process(LcdMenu* menu, const unsigned char command) override {
        DisplayInterface* display = menu->getDisplay();
        bool controlRes = false;
        switch (current) {
            case 0: controlRes = control0->process(menu, command); break;
            case 1: controlRes = control1->process(menu, command); break;
        }
        if (controlRes) {
            MenuItem::draw(display);
            return true;
        }
        if (display->getEditModeEnabled()) {
            switch (command) {
                case RIGHT:
                case ENTER:
                    if (getControlsCount() <= 1) {
                        return false;
                    }
                    current++;
                    current %= getControlsCount();
                    MenuItem::draw(display);
                    return true;
                case LEFT:
                    if (getControlsCount() <= 1) {
                        return false;
                    }
                    current--;
                    if (current < 0) {
                        current = getControlsCount();
                    }
                    MenuItem::draw(display);
                    return true;
                case BACK:
                    display->setEditModeEnabled(false);
                    if (callback != NULL) {
                        callback(control0->getValue(), control1->getValue());
                    }
                    display->clearBlinker();
                    printLog(F("ItemControl::exitEditMode"), control0->getValue());
                    return true;
                default:
                    return false;
            }
        } else {
            switch (command) {
                case ENTER:
                    display->setEditModeEnabled(true);
                    printLog(F("ItemControl::enterEditMode"), control0->getValue());
                    MenuItem::draw(display);
                    return true;
                default:
                    return false;
            }
        }
    }
};

template <typename T0>
class ItemControl<T0, void, void> : public MenuItem {
  protected:
    Control<T0>* control0;
    void (*callback)(T0);
    const uint8_t getControlsCount() {
        return 1;
    }

  public:
    ItemControl(
        const char* text,
        Control<T0>* control0,
        void (*callback)(T0))
        : MenuItem(text), control0(control0), callback(callback) {}
    void setValues(T0 value0) {
        control0->setValue(value0);
    }

  protected:
    void draw(DisplayInterface* display, uint8_t row) override {
        uint8_t maxCols = display->getMaxCols();
        uint8_t col = 1;
        static char* buf = new char[maxCols];
        display->drawItem(row, text, ':', buf);
        col += strlen(text) + 1;
        col += control0->draw(display, row, col);
        display->clearAfter(row, col);
        if (display->getEditModeEnabled()) {
            display->drawBlinker();
            display->resetBlinker(col - 1 - control0->getBlinkerOffset());
        }
    }

    bool process(LcdMenu* menu, const unsigned char command) override {
        DisplayInterface* display = menu->getDisplay();
        if (control0->process(menu, command)) {
            MenuItem::draw(display);
            return true;
        }
        if (display->getEditModeEnabled()) {
            switch (command) {
                case BACK:
                    display->setEditModeEnabled(false);
                    if (callback != NULL) {
                        callback(control0->getValue());
                    }
                    display->clearBlinker();
                    printLog(F("ItemControl::exitEditMode"), control0->getValue());
                    return true;
                default:
                    return false;
            }
        } else {
            switch (command) {
                case ENTER:
                    display->setEditModeEnabled(true);
                    printLog(F("ItemControl::enterEditMode"), control0->getValue());
                    MenuItem::draw(display);
                    return true;
                default:
                    return false;
            }
        }
    }
};

#endif  // ItemControl_H