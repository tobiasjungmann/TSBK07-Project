#pragma once

#include "LittleOBJLoader.h"
#include "gameobj.hpp"

namespace scn
{
  struct Terrain;
}

namespace obj
{
  class Fish : public CollidingObject
  {
  private:
    void moveSingleStep();

  protected:
    void updateModelToWorldRotation();
    inline mat3 orientationMtx() const noexcept override
    {
      return {
          0, -1, 0,
          0, 0, 1,
          -1, 0, 0};
    }

  public:
    Fish(Modelv2 mdl, vec3 pos, vec3 dir, float speed, vec3 up = vec3(0, 1, 0), vec3 size = vec3(1));
    void adaptToTerrain(scn::Terrain &terrain) override;
    void handleObjectCollision(CollidingObject *other) override;
    void update(scn::Scene &) override;
    void update(scn::Scene &, size_t) override;
    void collide(vec3 position, vec3 normalToCollisionPoint) override // BUG Tobias what is that fucntion ?
    {

    }
  private:
    float m_speed = 1.0f;
  };
}