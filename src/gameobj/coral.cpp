#include "coral.hpp"
#include "cmath"

namespace obj
{

    mat3 xRotation()
    {
        mat3 helper = IdentityMatrix();
        helper.m[4] = 0;
        helper.m[5] = 1;
        helper.m[7] = -1;
        helper.m[8] = 0;
        return helper;
    }

    mat3 zRotation()
    {
        mat3 helper = IdentityMatrix();
        helper.m[0] = 0;
        helper.m[1] = 1;
        helper.m[3] = -1;
        helper.m[4] = 0;
        return helper;
    }
    Coral::Coral(Model *m, vec3 pos, vec3 dir, vec3 up, vec3 size) : MoveableObject{m, pos, dir, up, size}
    { /*
         additionalModelRotation.m[0] = 0;
         additionalModelRotation.m[1] = -1;
         additionalModelRotation.m[4] = 0;
         additionalModelRotation.m[5] = 1;
         additionalModelRotation.m[6] = -1;
         additionalModelRotation.m[8] = 0;*/

        // z rotation - clockwise on the floor
        /*
                additionalModelRotation.m[0] =0;
                additionalModelRotation.m[1] = 1;
                additionalModelRotation.m[3] = -1;
                additionalModelRotation.m[4] = 0;*/

        // yaxis rotates around its up vector
        /**  additionalModelRotation.m[0] =0;
            additionalModelRotation.m[2] = 1;
            additionalModelRotation.m[6] = -1;
            additionalModelRotation.m[8] = 0;*/
        // last iteration
        /*    additionalModelRotation.m[4] =0;
            additionalModelRotation.m[5] = -1;
            additionalModelRotation.m[7] = 1;
            additionalModelRotation.m[8] = 0;*/
        // cos(90) = 0 and sin(90) = 1;
        /*
                additionalModelRotation.m[5] = 0;
                additionalModelRotation.m[6] = 0;
                additionalModelRotation.m[6] = -1;
                additionalModelRotation.m[9] = 1;
                additionalModelRotation.m[10] = 0;*/

        additionalModelRotation = IdentityMatrix();
        additionalModelRotation = additionalModelRotation * zRotation();
        MoveableObject::updateModelToWorldRotation(direction, up);

        // fromRotation();
        m2w.m[3] = position.x;
        m2w.m[7] = position.y;
        m2w.m[11] = position.z;
    };

    bool Coral::isCollision(const MoveableObject &other) const
    {
        return false;
    }

    void Coral::collide(vec3 position, vec3 normalToCollisionPoint)
    {
        // Coral cannot move - nothing changes
    }

    void Coral::moveSingleStep()
    {
        // Coral cannot move - nothing changes
    }

    void Coral::adaptToTerrain(scn::Terrain &terrain) {
        // Coral cannot move - nothing changes
    }
    void Coral::handleObjectCollision(MoveableObject *other) {
        // Coral cannot move - nothing changes
        // Everything goes trough an coral nothing changes
    }
}