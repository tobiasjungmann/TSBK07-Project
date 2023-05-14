#pragma once

#include "LittleOBJLoader.h"
#include "gameobj.hpp"
#include "../modelv2.hpp"

namespace obj
{
  class Coral : public ModelledObject
  {
  public:
    Coral(Modelv2 m, vec3 pos, vec3 dir, vec3 up = vec3(0,1,0), vec3 size = vec3(1));
    mat3 orientationMtx() const noexcept override;

  private:
    // void alignyaxis(vec3 newMovement, vec3 newUp);
  };
}