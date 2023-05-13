#include "gameobj.hpp"
#include "VectorUtils4.h"

namespace obj
{
  void CollidingObject::rotateHitbox(mat3 rotation)
  {
    mat3 baseHitbox = IdentityMatrix();
    baseHitbox.m[0] = m_size.x;
    baseHitbox.m[4] = m_size.y;
    baseHitbox.m[8] = m_size.z;
    m_hitbox = baseHitbox * rotation;
  }

  void ModelledObject::updateModelToWorldRotation(vec3 newDirection, vec3 newUp)
  {
    vec3 zaxis = CrossProduct(newUp, newDirection);
    vec3 yaxis = CrossProduct(newDirection, zaxis);
    m_direction = newDirection;
    m_upVector = newUp;

    if (m_position.inView)
    {
      if (auto m2w_p = m_model.matrix())
      {
        auto &m2w = *m2w_p;
        m2w.setLine(0, newDirection);
        m2w.setLine(1, yaxis);
        m2w.setLine(2, zaxis);
        mat3 combinedRotation = mat3(m2w) * orientationMtx();
        m2w = mat3tomat4(combinedRotation);

      }
    }

    /*mat4 scaling = IdentityMatrix();
    scaling.m[0] = sizeInDirection.x;
    scaling.m[5] = sizeInDirection.y;
    scaling.m[8] = sizeInDirection.z;
    m2w = m2w * scaling;*/
  }
}