#pragma once

#include "LittleOBJLoader.h"
#include "object.hpp"

namespace obj
{
  class Coral : public MoveableObject
  {
  public:
    Coral(Model *m, vec3 pos = vec3(1,0,0), vec3 dir = vec3(0),vec3 up=vec3(0,1,0));

    void moveSingleStep();
    bool isCollision(const MoveableObject &other) const;
    void collide(vec3 position, vec3 normalToCollisionPoint);

    private:
    void updateMovementVector(vec3 newMovement,vec3 newUp);
    void alignyaxis(vec3 newMovement, vec3 newUp);
  };
}