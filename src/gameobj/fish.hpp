#pragma once

#include "LittleOBJLoader.h"
#include "object.hpp"

namespace obj
{
  class Fish : public MoveableObject
  {
  public:
    Fish(Model *m, vec3 pos = vec3(1, 0, 0), vec3 dir = vec3(0), vec3 up = vec3(0, 1, 0),vec3 size=vec3(1,1,1));

    void moveSingleStep();
    bool isCollision(const MoveableObject &other) const;
    void collide(vec3 position, vec3 normalToCollisionPoint);
            void adaptToTerrain(scn::Terrain &terrain);
        void handleObjectCollision(MoveableObject * other);

void updateModelToWorldRotation(vec3 newMovement, vec3 newUp) ;
  private:
 void rotateHitbox(mat3 rotation);
  };
}