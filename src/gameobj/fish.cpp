#include "fish.hpp"
#include "cmath"

namespace obj
{
        void copyMat3InMat4(mat4 &m2w, mat3 rotation)
    {
        for (size_t i = 0; i < 9; i++)
        {
            m2w.m[i + i / 3] = rotation.m[i]; // leave out the field in the last column
        }
    }
    Fish::Fish(Model *m, vec3 pos, vec3 dir, vec3 up) : MoveableObject{m, pos, dir, up}
    {
        updateM2W(dir,up);
    };

    /*
    updatemovement vector - neuen übergebendann eine rotation in alle drei richtungen, wenn nötig
    */
    void Fish::updateM2W(vec3 newMovement, vec3 newUp) 
    {
        mat3 rotateHelper = IdentityMatrix();
        // rotate the fish model  so that the head is pointing along the x axis
        rotateHelper.m[0] = 0;
        rotateHelper.m[1] = -1;
        rotateHelper.m[4] = 0;
        rotateHelper.m[5] = 1;
        rotateHelper.m[6] = -1;
        rotateHelper.m[8] = 0;

        MoveableObject::updateM2W(direction,up);

        copyMat3InMat4(m2w, mat3(m2w) * rotateHelper);
    }

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
        position += direction * 0.05;
        m2w.m[3] = position.x;
        m2w.m[7] = position.y;
        m2w.m[11] = position.z;
    }
}