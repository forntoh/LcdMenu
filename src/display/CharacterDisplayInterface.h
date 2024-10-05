#pragma once

#include "DisplayInterface.h"

/**
 * @class CharacterDisplayInterface
 * @brief An interface for character-based display devices.
 *
 * This interface extends the DisplayInterface and provides additional
 * methods specific to character-based displays, such as creating custom
 * characters and controlling a blinker.
 *
 * @note This is an abstract class and cannot be instantiated directly.
 */
class CharacterDisplayInterface : public DisplayInterface {
  public:
    /**
     * @brief Virtual destructor for the CharacterDisplayInterface.
     */
    virtual ~CharacterDisplayInterface() {}

    /**
     * @brief Creates a custom character on the display.
     *
     * @param id The identifier for the custom character.
     * @param c A pointer to an array representing the custom character.
     */
    virtual void createChar(uint8_t id, uint8_t* c) = 0;

    /**
     * @brief Draws a blinker on the display.
     */
    virtual void drawBlinker() = 0;

    /**
     * @brief Clears the blinker from the display.
     */
    virtual void clearBlinker() = 0;
};
