#ifndef SSD1803A_I2C_STUB_H
#define SSD1803A_I2C_STUB_H
#include <stdint.h>
#include <string>

enum dispmode_t {
  DISPLAY_ON,
  DISPLAY_OFF,
  CURSOR_ON,
  CURSOR_OFF,
  BLINK_ON,
  BLINK_OFF
};

class SSD1803A_I2C {
public:
  bool clsCalled=false;
  bool displayCalled=false; int displayState=0;
  bool locateCalled=false; uint8_t row=0; uint8_t col=0;
  bool printCalled=false; std::string printed;
  bool writeCalled=false; uint8_t written=0;
  bool createCalled=false; uint8_t createId=0; uint8_t* createPtr=nullptr;
  void cls(){clsCalled=true;}
  void display(int state){displayCalled=true;displayState=state;}
  void locate(uint8_t r,uint8_t c){locateCalled=true;row=r;col=c;}
  size_t print(const char* text){printCalled=true;printed=text;return printed.size();}
  size_t write(uint8_t b){writeCalled=true;written=b;return 1;}
  void create(uint8_t id,uint8_t* ptr){createCalled=true;createId=id;createPtr=ptr;}
};
#endif
