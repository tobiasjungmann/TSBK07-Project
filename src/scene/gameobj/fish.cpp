#include "fish.hpp"
#include <cmath>
#include "../terrain.hpp"
#include "gameobj.hpp"

namespace obj
{

  Fish::Fish(Modelv2 mdl,
             vec3 pos,
             vec3 dir,
             float speed,
             vec3 up,
             vec3 size) : CollidingObject{mdl, pos, dir, up, size},
                          m_speed{speed}
  {
    if (mdl.matrix() == nullptr)
      mdl.matrix(IdentityMatrix());
    m_longestDistanceFromCenter = 8;
    // rotate the fish model  so that the head is pointing along the x axis
    updateModelToWorldRotation();
  };

  void Fish::moveSingleStep()
  {
    m_position += m_direction * m_speed;
    model().matrix()->m[3] = m_position().x; // BUG check that matrix is not nullptr
    model().matrix()->m[7] = m_position().y;
    model().matrix()->m[11] = m_position().z;
  }

  void Fish::updateModelToWorldRotation()
  {
    CollidingObject::updateModelToWorldRotation();
    model().matrix()->m[2] *= -1;
    model().matrix()->m[5] *= -1;// FIXME check here that it was like that
  }

  void Fish::update() {
    rotateHitbox(mat3(*model().matrix()) * orientationMtx());
    
    adaptToTerrain(mainScene.terrain);
    for (size_t u = 0; u < i; u++){
        auto test= allObjects[u].get();
        handleObjectCollision(allObjects[u].get());
    }
    moveSingleStep();
  }

  void Fish::adaptToTerrain(scn::Terrain &terrain)
  {
    float maxHeightDifference = -INFINITY;
    float maxHeightDifferenceToStart = 0;
    int indexOfSteepestPitch = 0;

    for (size_t i = 1; i < 5; i++)
    {
      vec3 futurePos = m_position + m_direction * m_speed * i * 10; // would touch it in the next five steps
      float futureHeight = terrain.computeHeight(futurePos.x, futurePos.z);

      if (maxHeightDifference < futureHeight - futurePos.y)
      {
        maxHeightDifference = futureHeight - futurePos.y;
        indexOfSteepestPitch = i;
        maxHeightDifferenceToStart = futureHeight - m_position().y;
      }
    }

    if (maxHeightDifference > -m_longestDistanceFromCenter + 1)
    {
      m_direction().y = std::abs(maxHeightDifferenceToStart + m_longestDistanceFromCenter) / indexOfSteepestPitch;
      m_direction().x = indexOfSteepestPitch * m_direction().x;
      m_direction().z = indexOfSteepestPitch * m_direction().z;

      // movement must be adapted to prevent a collision increase y movement  - rotate the vector to match the new angle
      m_direction = normalize(m_direction);
      updateModelToWorldRotation();
    }
    else if (maxHeightDifference < -m_longestDistanceFromCenter && m_direction().y > 0)
    {

      m_direction().y = 0;

      m_direction = normalize(m_direction);
      updateModelToWorldRotation();
    }
    // TODO also check that they do not move out of the water on the top
  }
  void Fish::handleObjectCollision(CollidingObject *other) {}
}