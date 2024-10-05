#pragma once

#include "DisplayInterface.h"

class CharacterDisplayInterface : public DisplayInterface {
  public:
    virtual ~CharacterDisplayInterface() {}

    virtual void createChar(uint8_t id, uint8_t* c) = 0;

    virtual void drawBlinker() = 0;

    virtual void clearBlinker() = 0;
};
