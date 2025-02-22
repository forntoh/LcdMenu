#pragma once

/**
 * @brief A wrapper struct that holds a reference to a value of type T.
 *
 * This struct is used to create a reference wrapper for a value of type T.
 * It provides a convenient way to pass references around, especially in
 * generic programming.
 *
 * @tparam T The type of the value to be referenced.
 */
template <typename T>
struct Ref {
    T& value;
    Ref(T& value) : value(value) {}
};

/**
 * @brief A simple wrapper struct for a pointer of type T.
 *
 * This struct is used to wrap a pointer of any type T, providing a
 * convenient way to manage and access the pointer.
 *
 * @tparam T The type of the object that the pointer points to.
 */
template <typename T>
struct Ptr {
    T* value;
    Ptr(T* value) : value(value) {}
};