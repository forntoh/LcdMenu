#include <ArduinoUnitTests.h>
#include <utils.h>

unittest(concat_three) {
    char* result = concat("Lcd", 'M', "enu");
    assertEqual("LcdMenu", result);
}

unittest(concat_two) {
    char* result = concat("Lcd", 'M');
    assertEqual("LcdM", result);
}

unittest(substring) {
    char* str = "Hello World";
    char* expected = "World";
    assertEqual(expected, substring(str, 6, 5));
}

unittest(remove) {
    char str[] = "hello world";
    remove(str, 2, 3);
    assertEqual("he world", str);
}

unittest_main()