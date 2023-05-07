#pragma once

#include "LittleOBJLoader.h"
#include "MoveableObject.hpp"

namespace obj
{
  class Fish : public MoveableObject
  {
  public:
    Fish(Model *m, vec3 pos = vec3(0), vec3 dir = vec3(0));

    void moveSingleStep();
    mat4 getM2W();
    bool isCollision(const MoveableObject &other) const;
    void collide(vec3 position, vec3 normalToCollisionPoint);
    private:
    void updateMovementVector(vec3 newMovement,vec3 newUp);
  };
}