#include "event.hpp"

namespace sm {


}

namespace evt {
  void Context::setMousePosition(int x, int y) noexcept
  {
    mouse.update(x, y);
  }

  void Context::setPressedKeys(bool forward, bool left, bool back, bool right, bool color) noexcept
  {
    keys.w = forward;
    keys.a = left;
    keys.s = back;
    keys.d = right;
    keys.c = color;
  }
}

