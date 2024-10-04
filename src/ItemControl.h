#ifndef ItemControl_H
#define ItemControl_H

#include "Control.h"
#include "LcdMenu.h"
#include "MenuItem.h"
#include "printf.h"
#include <utils/utils.h>

class ItemControlBaseMany : public MenuItem {
  protected:
    Control** controls = NULL;
    const uint8_t size = 0;
    uint8_t current = 0;

  public:
    ItemControlBaseMany(const char* text, Control** controls, const uint8_t size, uint8_t current = 0)
        : MenuItem(text), controls(controls), size(size), current(current) {}
    uint8_t getCurrent() {
        return current;
    }
    void setCurrent(uint8_t newCurrent) {
        current = newCurrent;
    }

  protected:
    void reset() {
        current = 0;
    }
    void draw(DisplayInterface* display, uint8_t row) override {
        uint8_t maxCols = display->getMaxCols() - 1;
        uint8_t col = 1;
        col += display->drawAt(row, col, (char*)this->text);
        col += display->drawAt(row, col, ':');
        uint8_t limit = maxCols - col;
        Serial.print("Call draw on (row=");
        Serial.print(row);
        Serial.print(", col=");
        Serial.print(col);
        Serial.print(", limit=");
        Serial.print(limit);
        Serial.println(")");
        char buffer[limit];
        buffer[0] = '\0';  // Cleanup
        uint8_t accum = 0;
        int rightTrim = -1;
        uint8_t blinkerPosition = 0;
        if (!display->getEditModeEnabled()) {
            char text[20];
            for (size_t i = 0; i < size; i++) {
                accum += controls[i]->draw2(text, 20);
                rightTrim = accum - limit;
                Serial.print("Right iteration [");
                Serial.print(i);
                Serial.print("], text=[");
                Serial.print(text);
                Serial.print("], rightTrim=[");
                Serial.print(rightTrim);
                Serial.println("]");
                if (rightTrim >= 0) {
                    Serial.print("rightTrim exceed ");
                    Serial.println(rightTrim);
                    substring(text, 0, strlen(text) - rightTrim, text);
                    concat(buffer, text, buffer);
                    break;
                }
                concat(buffer, text, buffer);
            }
        } else {
            char texts[size][20];
            int left = current;
            int right = current + 1;
            int leftTrim = -1;
            // From current to left
            Serial.println("Left iteration start");
            while (left >= 0 && leftTrim < 0) {
                accum += controls[left]->draw2(texts[left], 20);
                leftTrim = accum - limit;
                Serial.print("Left iteration [");
                Serial.print(left);
                Serial.print("], texts[left]=[");
                Serial.print(texts[left]);
                Serial.print("], leftTrim=[");
                Serial.print(leftTrim);
                Serial.println("]");
                left--;
                if (leftTrim >= 0) {
                    Serial.print("leftTrim exceed ");
                    Serial.println(leftTrim);
                    break;
                }
            }
            Serial.println("Left iteration end");
            if (leftTrim < 0 && current < size - 1) {
                // From current + 1 to right
                Serial.println("Right iteration start");
                while (right < (int)size && rightTrim < 0) {
                    accum += controls[right]->draw2(texts[right], 20);
                    rightTrim = accum - limit;
                    Serial.print("Right iteration [");
                    Serial.print(right);
                    Serial.print("], texts[right]=[");
                    Serial.print(texts[right]);
                    Serial.print("], rightTrim=[");
                    Serial.print(rightTrim);
                    Serial.println("]");
                    right++;
                    if (rightTrim >= 0) {
                        Serial.print("rightTrim exceed ");
                        Serial.println(rightTrim);
                        break;
                    }
                }
                Serial.println("Right iteration end");
            }
            Serial.println("Constructing...");
            Serial.print("left=");
            Serial.print(left);
            Serial.print(", right=");
            Serial.println(right);
            for (uint8_t i = left + 1; i < right; i++) {
                char* text = texts[i];
                Serial.print("Handle (");
                Serial.print(i);
                Serial.print(") ");
                Serial.println(text);
                if (i == left + 1 && leftTrim > 0) {
                    substring(text, leftTrim, strlen(text) - leftTrim, text);
                    Serial.print("After left substring [");
                    Serial.print(text);
                    Serial.println("]");
                }
                if (i == right - 1 && rightTrim > 0) {
                    substring(text, 0, strlen(text) - rightTrim, text);
                    Serial.print("After right substring [");
                    Serial.print(text);
                    Serial.println("]");
                }
                concat(buffer, text, buffer);
                if (i == current) {
                    Serial.print("Cuurent widget, buffer=[");
                    Serial.print(buffer);
                    Serial.print("], offset=");
                    Serial.println(controls[i]->getBlinkerOffset());
                    blinkerPosition = strlen(buffer) - controls[i]->getBlinkerOffset() - 1;
                    Serial.print("blinker global offset = ");
                    Serial.println(blinkerPosition);
                }
            }
        }
        Serial.print("Constructed [");
        Serial.print(buffer);
        Serial.println("]");
        display->drawAt(row, col, buffer);
        if (rightTrim < 0) {
            Serial.print("Call clearAfter ");
            Serial.println(col + strlen(buffer));
            display->clearAfter(row, col + strlen(buffer) - 1);
        }
        if (display->getEditModeEnabled()) {
            display->drawBlinker();
            Serial.print("blinkerPosition = ");
            Serial.println(col + blinkerPosition);
            display->resetBlinker(col + blinkerPosition);
        }
        Serial.println("");
        Serial.println("");
    }

    virtual void triggerCallback() = 0;

    bool process(LcdMenu* menu, const unsigned char command) override {
        DisplayInterface* display = menu->getDisplay();
        if (controls[current]->process(menu, command)) {
            MenuItem::draw(display);
            return true;
        }
        if (display->getEditModeEnabled()) {
            switch (command) {
                case RIGHT:
                case ENTER:
                    current = (current + 1) % this->size;
                    MenuItem::draw(display);
                    return true;
                case LEFT:
                    current--;
                    if (current < 0) {
                        current = this->size;
                    }
                    MenuItem::draw(display);
                    return true;
                case BACK:
                    display->setEditModeEnabled(false);
                    reset();
                    triggerCallback();
                    display->clearBlinker();
                    MenuItem::draw(display);
                    printLog(F("ItemControl::exitEditMode"), this->text);
                    return true;
                default:
                    return false;
            }
        } else {
            switch (command) {
                case ENTER:
                    display->setEditModeEnabled(true);
                    printLog(F("ItemControl::enterEditMode"), this->text);
                    MenuItem::draw(display);
                    return true;
                default:
                    return false;
            }
        }
    }
};

template <typename T0, typename T1 = void, typename T2 = void>
class ItemControl : public ItemControlBaseMany {
  protected:
    ControlValue<T0>* control0;
    ControlValue<T1>* control1;
    ControlValue<T2>* control2;
    void (*callback)(T0, T1, T2);
    static Control** toArray(ControlValue<T0>* control0, ControlValue<T1>* control1, ControlValue<T2>* control2) {
        static Control* array[] = {control0, control1, control2};
        return array;
    }
    void triggerCallback() override {
        if (callback != NULL) {
            callback(
                static_cast<ControlValue<T0>*>(controls[0])->getValue(),
                static_cast<ControlValue<T1>*>(controls[1])->getValue(),
                static_cast<ControlValue<T2>*>(controls[2])->getValue());
        }
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
        ControlValue<T0>* control0,
        ControlValue<T1>* control1,
        ControlValue<T2>* control2,
        void (*callback)(T0, T1, T2))
        : ItemControlBaseMany(text, toArray(control0, control1, control2), 3), callback(callback) {}

    void setValues(T0 value0, T1 value1, T2 value2) {
        static_cast<ControlValue<T0>*>(controls[0])->setValue(value0);
        static_cast<ControlValue<T1>*>(controls[1])->setValue(value1);
        static_cast<ControlValue<T2>*>(controls[2])->setValue(value2);
    }
};

template <typename T0, typename T1>
class ItemControl<T0, T1, void> : public ItemControlBaseMany {
  protected:
    void (*callback)(T0, T1) = NULL;
    static Control** toArray(ControlValue<T0>* control0, ControlValue<T1>* control1) {
        static Control* array[] = {control0, control1};
        return array;
    }
    void triggerCallback() override {
        if (callback != NULL) {
            callback(
                static_cast<ControlValue<T0>*>(controls[0])->getValue(),
                static_cast<ControlValue<T1>*>(controls[1])->getValue());
        }
    }

  public:
    ItemControl(
        const char* text,
        ControlValue<T0>* control0,
        ControlValue<T1>* control1,
        void (*callback)(T0, T1))
        : ItemControlBaseMany(text, toArray(control0, control1), 2), callback(callback) {}

    void setValues(T0 value0, T1 value1) {
        static_cast<ControlValue<T0>*>(controls[0])->setValue(value0);
        static_cast<ControlValue<T1>*>(controls[1])->setValue(value1);
    }
};

template <typename T0>
class ItemControl<T0, void, void> : public MenuItem {
  protected:
    ControlValue<T0>* control;
    void (*callback)(T0);

  public:
    ItemControl(
        const char* text,
        ControlValue<T0>* control,
        void (*callback)(T0))
        : MenuItem(text), control(control), callback(callback) {}
    void setValues(T0 value0) {
        control->setValue(value0);
    }

  protected:
    void draw(DisplayInterface* display, uint8_t row) override {
        uint8_t col = 1;
        col += display->drawAt(row, col, (char*)this->text);
        col += display->drawAt(row, col, ':');

        col += control->draw(display, row, col);
        // display->clearAfter(row, col);
        if (display->getEditModeEnabled()) {
            display->drawBlinker();
            display->resetBlinker(col - 1 - control->getBlinkerOffset());
        }
    }

    bool process(LcdMenu* menu, const unsigned char command) override {
        DisplayInterface* display = menu->getDisplay();
        if (control->process(menu, command)) {
            MenuItem::draw(display);
            return true;
        }
        if (display->getEditModeEnabled()) {
            switch (command) {
                case BACK:
                    display->setEditModeEnabled(false);
                    if (callback != NULL) {
                        callback(control->getValue());
                    }
                    display->clearBlinker();
                    printLog(F("ItemControl::exitEditMode"), this->text);
                    return true;
                default:
                    return false;
            }
        } else {
            switch (command) {
                case ENTER:
                    display->setEditModeEnabled(true);
                    printLog(F("ItemControl::enterEditMode"), this->text);
                    MenuItem::draw(display);
                    return true;
                default:
                    return false;
            }
        }
    }
};

#endif  // ItemControl_H