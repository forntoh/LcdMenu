#pragma once

#include "InputInterface.h"

/**
 * @class JoystickAdapter
 * @brief Adapter class to handle analog joystick input for an LCD menu.
 *
 * This adapter reads the horizontal and vertical axes of a joystick and
 * converts them into menu commands.
 *
 * The default centre value is assumed to be around 512 with a configurable
 * margin to detect direction changes.
 */
class JoystickAdapter : public InputInterface {
  private:
    uint8_t hPin;     ///< Analog pin for horizontal axis
    uint8_t vPin;     ///< Analog pin for vertical axis
    uint16_t margin;  ///< Dead zone around the centre value
    byte leftCmd;     ///< Command for moving left
    byte rightCmd;    ///< Command for moving right
    byte upCmd;       ///< Command for moving up
    byte downCmd;     ///< Command for moving down

    // store last states so we only send commands when the direction changes
    int8_t lastHoriz = 0;
    int8_t lastVert = 0;

    inline int8_t readAxis(int value) {
        if (value < 512 - margin) return -1;
        if (value > 512 + margin) return 1;
        return 0;
    }

  public:
    /**
     * Construct a new JoystickAdapter.
     *
     * The adapter can be customised by providing command codes for each
     * direction. By default the horizontal axis maps to ``BACK`` and
     * ``RIGHT`` and the vertical axis to ``UP`` and ``DOWN``. Pressing the
     * joystick triggers ``ENTER``.
     *
     * @param menu Pointer to the LcdMenu instance
     * @param horizontalPin Analog pin connected to the horizontal axis
     * @param verticalPin Analog pin connected to the vertical axis
     * @param margin Dead zone margin around the centre (default ``100``)
     * @param leftCmd Command for the left direction (default ``BACK``)
     * @param rightCmd Command for the right direction (default ``RIGHT``)
     * @param upCmd Command for the up direction (default ``UP``)
     * @param downCmd Command for the down direction (default ``DOWN``)
     */
    JoystickAdapter(LcdMenu* menu,
                    uint8_t horizontalPin,
                    uint8_t verticalPin,
                    uint16_t margin = 100,
                    byte leftCmd = BACK,
                    byte rightCmd = RIGHT,
                    byte upCmd = UP,
                    byte downCmd = DOWN)
        : InputInterface(menu), hPin(horizontalPin), vPin(verticalPin),
          margin(margin), leftCmd(leftCmd),
          rightCmd(rightCmd), upCmd(upCmd), downCmd(downCmd) {}

    void observe() override {
        int h = analogRead(hPin);
        int v = analogRead(vPin);
        int8_t horiz = readAxis(h);
        int8_t vert = readAxis(v);

        if (horiz != lastHoriz) {
            lastHoriz = horiz;
            if (horiz == -1) {
                menu->process(leftCmd);
            } else if (horiz == 1) {
                menu->process(rightCmd);
            }
        }

        if (vert != lastVert) {
            lastVert = vert;
            if (vert == -1) {
                menu->process(upCmd);
            } else if (vert == 1) {
                menu->process(downCmd);
            }
        }
    }
};
