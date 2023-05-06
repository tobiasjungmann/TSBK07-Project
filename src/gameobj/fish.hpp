#pragma once

#include "LittleOBJLoader.h"
#include "MoveableObject.hpp"

namespace obj
{
  class Fish : public MoveableObject
  {
  public:
    Fish(Model *m, vec3 pos = vec3(0), vec3 dir = vec3(0));

        bool isCollision(const MoveableObject &other) const;
        void collide(vec3 position, vec3 normalToCollisionPoint)=0;
        
  private:
    std::string modelName;
    vec3 pos;
    vec3 vel;
    vec3 acc;
  };
}