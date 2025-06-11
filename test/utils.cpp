#include <vector>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <string>
#include <type_traits>
#include <utils/constants.h>
#include <utils/utils.h>

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

unittest(concat_same_strings) {
    char result[10];
    concat("Hello", '!', result);
    assertEqual("Hello!", result);
}

unittest(concat_empty_string) {
    char result[5];
    concat("", 'A', result);
    assertEqual("A", result);
}

unittest(concat_two_strings) {
    char result[20];
    concat("Hello", "World", result);
    assertEqual("HelloWorld", result);
}

unittest(concat_long_result) {
    char result[15];
    concat("Hello", ' ', "World!", result);
    assertEqual("Hello World!", result);
}

unittest(substring_full_string) {
    char str[] = "Hello World";
    char expected[] = "Hello World";
    char result[12];
    substring(str, 0, 11, result);
    assertEqual(expected, result);
}

unittest(substring_start_index_zero) {
    char str[] = "Hello World";
    char expected[] = "Hello";
    char result[6];
    substring(str, 0, 5, result);
    assertEqual(expected, result);
}

unittest(remove_no_removal) {
    char str[] = "Hello";
    remove(str, 2, 0);
    assertEqual("Hello", str);
}

unittest(remove_entire_string) {
    char str[] = "Hello";
    remove(str, 0, 5);
    assertEqual("", str);
}

unittest(remove_partial_string) {
    char str[] = "Hello World";
    remove(str, 6, 5);
    assertEqual("Hello ", str);
}

unittest_main()