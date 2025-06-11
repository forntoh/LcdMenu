#ifndef TEST_STUB_SIMPLE_ROTARY_H
#define TEST_STUB_SIMPLE_ROTARY_H
#include <stdint.h>
class SimpleRotary {
  public:
    virtual uint8_t rotate() { return 0; }
    virtual uint8_t pushType(int) { return 0; }
};
#endif
