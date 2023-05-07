#include "fish.hpp"
#include "cmath"

namespace obj
{

    void copyMat3InMat4(mat4 &m2w, mat3 rotation)
    {
        for (size_t i = 0; i < 9; i++)
        {
            m2w.m[i + i / 3] = rotation.m[i]; // leave out the field in hte last column
        }
    }

    Fish::Fish(Model *m, vec3 pos, vec3 dir) : MoveableObject{m, pos, dir}
    {
        float rotation_factor = M_1_PI*5;        
        mat3 rotateZ=IdentityMatrix();
        mat3 rotateX=IdentityMatrix();

        rotateZ.m[0] = cos(rotation_factor);
        rotateZ.m[2] = sin(rotation_factor);
        rotateZ.m[6] = -sin(rotation_factor);
        rotateZ.m[8] = cos(rotation_factor);

        /* rotation to put it alongside in the water*/
        rotateX.m[0] = cos(rotation_factor);
        rotateX.m[1] = -sin(rotation_factor);
        rotateX.m[3] = sin(rotation_factor);
        rotateX.m[4] = cos(rotation_factor);
        rotateZ=rotateX*rotateZ;
        copyMat3InMat4(m2w,rotateZ);

    };

    bool Fish::isCollision(const MoveableObject &other) const
    {
        return false;
    }

    void Fish::collide(vec3 position, vec3 normalToCollisionPoint)
    {
        // TODO add reflection on normal
        // TODO also alter the up vector for non 2D reflections
        // copyMat3InM2W(m2w,lookAt(position, position + movementDirection, up));
    }

    void Fish::moveSingleStep()
    {
        position += movementDirection * 0.1;
        m2w.m[3] = position.x;
        m2w.m[7] = position.y;
        m2w.m[11] = position.z;
    }

    mat4 Fish::getM2W()
    {
        return m2w;
    }
}