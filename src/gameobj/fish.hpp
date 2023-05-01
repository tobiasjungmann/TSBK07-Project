#pragma once

#include "LittleOBJLoader.h"

namespace obj {
  class Fish {
    Fish(std::string modelName, vec3 pos = vec3(0));
    private:
      std::string modelName; 
      vec3 pos;
      vec3 vel;
      vec3 acc;
  };
}