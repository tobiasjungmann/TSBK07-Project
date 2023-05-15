#pragma once

#include <memory>
#include <unordered_set>
#include <cstdlib>
#include <cmath>
#include <cassert>
#include <limits>
#include "VectorUtils4.h"
#include "MicroGlut.h"

namespace evt
{
  struct Context;
  struct Mouse;
  struct MovementKeys;

  struct Mouse
  {
    vec2 currPos = vec2(450, 450); // FIXME here you should make use of your values in constants::
    vec2 prevPos = vec2(450, 450);

    void update(int x, int y)
    {
      prevPos = currPos;
      currPos.x = x;
      currPos.y = y;
    }
  };

  struct MovementKeys
  {
    union
    {
      bool w;
      bool forward;
    };
    union
    {
      bool a;
      bool left;
    };
    union
    {
      bool s;
      bool back;
    };
    union
    {
      bool d;
      bool right;
    };

    union
    {
      bool c;
      bool color;
    };
  };

  struct Context
  {
    evt::Mouse mouse;
    evt::MovementKeys keys;

    /**
     * @brief Set the Mouse position based on the given location
     *
     * @param x position of the mouse
     * @param y position of the mouse
     */
    void setMousePosition(int x, int y) noexcept;

    /**
     * @brief Indicates, if one or more of the keys for forward, left, down and right are currently pressed.
     *
     * @param forward - true if the key responsible for the movement is pressed.
     * @param left - true if the key responsible for the movement is pressed.
     * @param back - true if the key responsible for the movement is pressed.
     * @param right - true if the key responsible for the movement is pressed.
     */
    void setPressedKeys(bool forward, bool left, bool back, bool right, bool color) noexcept;
  };
}