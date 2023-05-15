#include "coral.hpp"
#include <cmath>

namespace obj
{
    mat3 Coral::orientationMtx() const noexcept
    {
        mat3 helper = IdentityMatrix();
        helper.m[0] = 0;
        helper.m[1] = 1;
        helper.m[3] = -1;
        helper.m[4] = 0;
        return helper;
    }

    Coral::Coral(Modelv2 m, vec3 pos, vec3 dir, vec3 up, vec3 size)
        : ModelledObject(m, pos, dir, up, size)
    {
        if (not m.matrix())
        {
            mat4 matr = IdentityMatrix();
            m.matrix(matr);
        }
        ModelledObject::updateModelToWorldRotation(m_direction, m_upVector);
        // move the stemp to the ground
        m_position = m_position - normalize(m_direction);

        if (auto mtx = m.matrix())
        {
            mtx->m[3] = m_position().x;
            mtx->m[7] = m_position().y;
            mtx->m[11] = m_position().z;
        }
    };

}