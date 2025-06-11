#ifndef LIQUIDCRYSTAL_STUB_H
#define LIQUIDCRYSTAL_STUB_H
#include <stdint.h>
#include <string>
class LiquidCrystal {
public:
  bool beginCalled=false; uint8_t beginCols=0; uint8_t beginRows=0;
  bool clearCalled=false;
  bool displayCalled=false; bool noDisplayCalled=false;
  bool blinkCalled=false; bool noBlinkCalled=false;
  bool setCursorCalled=false; uint8_t cursorCol=0; uint8_t cursorRow=0;
  bool printCalled=false; std::string printed;
  bool writeCalled=false; uint8_t written=0;
  bool createCharCalled=false; uint8_t createCharId=0; uint8_t* createCharPtr=nullptr;
  void begin(uint8_t c, uint8_t r){beginCalled=true;beginCols=c;beginRows=r;}
  void clear(){clearCalled=true;}
  void display(){displayCalled=true;}
  void noDisplay(){noDisplayCalled=true;}
  void blink(){blinkCalled=true;}
  void noBlink(){noBlinkCalled=true;}
  void setCursor(uint8_t col,uint8_t row){setCursorCalled=true;cursorCol=col;cursorRow=row;}
  size_t print(const char* text){printCalled=true;printed=text;return printed.size();}
  size_t write(uint8_t b){writeCalled=true;written=b;return 1;}
  void createChar(uint8_t id,uint8_t* ptr){createCharCalled=true;createCharId=id;createCharPtr=ptr;}
};
#endif
