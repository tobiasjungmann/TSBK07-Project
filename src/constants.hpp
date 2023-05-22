#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace constants
{
  enum class Key : char {
    Forward = 'w',
    Left = 'a',
    Back = 's',
    Right = 'd',
    Color = 'c'
  };
  constexpr int display_size{900};
}
#endif