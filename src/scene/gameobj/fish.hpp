#pragma once

#include "LittleOBJLoader.h"
#include "gameobj.hpp"

namespace obj
{
  class Fish : public GameObj
  {
  public:
    Fish(std::string modelName, vec3 pos = vec3(0));
    // FIXME call Base class constructor

  private:
    vec3 pos;
    vec3 vel;
    vec3 acc;
  };
}