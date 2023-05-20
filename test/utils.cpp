#include <ArduinoUnitTests.h>
#include <utils.h>

unittest(concat_three) {
    char* result = concat("Lcd", 'M', "enu");
    assertEqual("LcdMenu", result);
    delete (result);
}

unittest(concat_two) {
    char* result = concat("Lcd", 'M');
    assertEqual("LcdM", result);
    delete (result);
}

unittest(substring) {
    char* str = "Hello World";
    char* expected = "World";
    char* result = substring(str, 6, 5);
    assertEqual(expected, result);
    delete (result);
}

unittest(remove) {
    char str[] = "hello world";
    remove(str, 2, 3);
    assertEqual("he world", str);
}

unittest_main()