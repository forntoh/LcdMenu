#ifndef ARDUINO_UNIT_TESTS_H
#define ARDUINO_UNIT_TESTS_H
#include <vector>
#include <functional>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <string>
#include <type_traits>

struct TestCase {
    const char* name;
    void (*func)();
};

static std::vector<TestCase> __tests;

#define unittest(name) \
    void name(); \
    struct __reg_##name { __reg_##name() { __tests.push_back({#name, name}); } } __reg_instance_##name; \
    void name()

template<typename A, typename B>
inline typename std::enable_if<
    std::is_convertible<A, const char*>::value && std::is_convertible<B, const char*>::value,
    bool>::type __eq(const A& a, const B& b) {
    return std::strcmp(static_cast<const char*>(a), static_cast<const char*>(b)) == 0;
}

template<typename A, typename B>
inline typename std::enable_if<
    !(std::is_convertible<A, const char*>::value && std::is_convertible<B, const char*>::value),
    bool>::type __eq(const A& a, const B& b) {
    return a == b;
}

#define assertTrue(cond) do { if(!(cond)) { \
    std::cerr << __FILE__ << ":" << __LINE__ << " assertTrue(" << #cond << ") failed" << std::endl; \
    std::exit(1); } } while(0)

#define assertEqual(expected, actual) do { auto _exp = (expected); auto _act = (actual); \
    if(!__eq(_exp, _act)) { \
        std::cerr << __FILE__ << ":" << __LINE__ << " assertEqual failed" << std::endl; \
        std::exit(1); } } while(0)

#define unittest_main() \
    int main() { \
        for(const auto& t : __tests) { \
            std::cout << t.name << "..." << std::endl; \
            t.func(); \
        } \
        std::cout << __tests.size() << " tests ran" << std::endl; \
        return 0; \
    }

#endif // ARDUINO_UNIT_TESTS_H
