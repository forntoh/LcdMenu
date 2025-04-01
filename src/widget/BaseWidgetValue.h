// Creator: @ShishkinDmitriy
#pragma once

#include "BaseWidget.h"
#include "utils/std.h"
#include <functional>

class LcdMenu;

/**
 * @brief A wrapper for reference values that provides value semantics.
 *
 * This struct allows widgets to work with references while maintaining
 * a consistent interface with value types.
 *
 * @tparam T The type being referenced (must be a non-reference type)
 */
template <typename T>
struct Ref {
    T& value;
    Ref(T& value) : value(value) {}
    bool operator==(T const& other) const noexcept { return value == other; }
    bool operator!=(T const& other) const noexcept { return value != other; }
    bool operator>(T const& other) const noexcept { return value > other; }
    bool operator<(T const& other) const noexcept { return value < other; }
    bool operator>=(T const& other) const noexcept { return value >= other; }
    bool operator<=(T const& other) const noexcept { return value <= other; }
    Ref<T> operator++(int) noexcept {
        this->value++;
        return *this;
    }
    Ref<T> operator--(int) noexcept {
        this->value--;
        return *this;
    }
    Ref<T> operator+(T other) noexcept {
        this->value += other;
        return *this;
    }
    Ref<T> operator-(T other) noexcept {
        this->value -= other;
        return *this;
    }
    Ref<T>& operator=(T other) noexcept {
        this->value = other;
        return *this;
    }
    Ref<T>& operator=(Ref<T> other) noexcept {
        this->value = other.value;
        return *this;
    }
    operator T() const noexcept { return value; }  // Cast to T
};

/**
 * @class BaseWidgetValue
 * @brief Base class for widget holding some value.
 * @tparam T The type of value this widget holds
 * @see BaseWidget
 */
template <typename T>
class BaseWidgetValue : public BaseWidget {
  public:
    using CallbackType = std::function<void(const T&)>;

  protected:
    T value;
    const char* format = nullptr;
    CallbackType callback = nullptr;

  public:
    BaseWidgetValue(
        const T& value,
        const char* format,
        const uint8_t cursorOffset = 0,
        CallbackType callback = nullptr)
        : BaseWidget(cursorOffset), value(value), format(format), callback(callback) {}
    /**
     * @brief Retrieve current value.
     */
    const T& getValue() const { return value; }
    /**
     * @brief Sets the value.
     *
     * @param newValue The value to set.
     * @note You need to call `LcdMenu::refresh` after this method to see the changes.
     */
    virtual void setValue(const T& newValue) {
        if (value != newValue) {
            value = newValue;
            handleChange();
        }
    }

  protected:
    /**
     * @brief Draw the widget into specified buffer.
     *
     * @param buffer the buffer where widget will be drawn
     * @param start the index where to start drawing in the buffer
     */
    uint8_t draw(char* buffer, const uint8_t start) override {
        if (start >= ITEM_DRAW_BUFFER_SIZE) return 0;
        return snprintf(buffer + start, ITEM_DRAW_BUFFER_SIZE - start, format, value);
    }

    /**
     * @brief Process a command for this widget
     * @param menu Pointer to the menu instance
     * @param command The command to process
     * @return true if the command was handled, false otherwise
     */
    bool process(LcdMenu* menu, unsigned char command) override = 0;

    void handleChange() {
        if (callback != nullptr) {
            callback(value);
        }
    }

  public:
    ~BaseWidgetValue() override = default;
};