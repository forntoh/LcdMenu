#ifndef ITEM_WIDGET_H
#define ITEM_WIDGET_H

#include "BaseItemManyWidgets.h"
#include "widget/BaseWidgetValue.h"

// Custom index_sequence implementation for C++11
template <size_t... Is>
struct index_sequence {};

template <size_t N, size_t... Is>
struct make_index_sequence : make_index_sequence<N - 1, N - 1, Is...> {};

template <size_t... Is>
struct make_index_sequence<0, Is...> {
    using type = index_sequence<Is...>;
};

// Updated ItemWidget class supporting zero or more widgets
template <typename... Ts>
class ItemWidget : public BaseItemManyWidgets {
  protected:
    using CallbackType = void (*)(Ts...);
    CallbackType callback = nullptr;

    void handleCommit() override {
        if (callback != nullptr) {
            invokeCallback(typename make_index_sequence<sizeof...(Ts)>::type{});
        }
    }

    template <size_t... Is>
    void invokeCallback(index_sequence<Is...>) {
        callback(static_cast<BaseWidgetValue<Ts>*>(widgets[Is])->getValue()...);
    }

  public:
    // Constructor for one or more widgets
    ItemWidget(const char* text, BaseWidgetValue<Ts>*... widgetPtrs, CallbackType callback = nullptr)
        : BaseItemManyWidgets(
              text,
              // clang-format off
              new BaseWidget* [sizeof...(Ts)] { widgetPtrs... },
              // clang-format on
              sizeof...(Ts)),
          callback(callback) {}

    void setValues(Ts... values) {
        setValuesImpl(typename make_index_sequence<sizeof...(Ts)>::type{}, values...);
    }

  private:
    template <size_t... Is>
    void setValuesImpl(index_sequence<Is...>, Ts... values) {
        // Using a dummy array to expand the parameter pack and call setValue on each widget
        int dummy[] = {(static_cast<BaseWidgetValue<Ts>*>(widgets[Is])->setValue(values), 0)...};
        static_cast<void>(dummy);  // Avoid unused variable warning
    }
};

// ############################################################################
// # Shortcuts for creating new ItemWidget instances
// ############################################################################

#define ITEM_3_WIDGET(text, widget0, widget1, widget2, callback, type0, type1, type2) \
    new ItemWidget<type0, type1, type2>(text, widget0, widget1, widget2, callback)

#define ITEM_2_WIDGET(text, widget0, widget1, callback, type0, type1) \
    new ItemWidget<type0, type1>(text, widget0, widget1, callback)

#define ITEM_1_WIDGET(text, widget, callback, type) \
    new ItemWidget<type>(text, widget, callback)

#endif  // ITEM_WIDGET_H