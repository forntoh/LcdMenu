#include <ArduinoUnitTests.h>
#include <utils.h>

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