#include "coral.hpp"
#include "cmath"

namespace obj
{
    void Coral::alignyaxis(vec3 newMovement, vec3 newUp)
    {
        vec3 first = CrossProduct(vec3(1, 0, 0), newMovement);
        vec3 second = CrossProduct(newMovement, first);

        setVectorInLine(m2w, first, 0);
        setVectorInLine(m2w, newMovement, 1);
        setVectorInLine(m2w, second, 2);

        direction = newMovement;
        up = newUp;
    }

    Coral::Coral(Model *m, vec3 pos, vec3 dir, vec3 up) : MoveableObject{m, pos, dir, up}
    {
        m2w = IdentityMatrix();
        m2w.m[3] = position.x;
        m2w.m[7] = position.y;
        m2w.m[11] = position.z;

        alignyaxis(dir, vec3(0));
        /*  mat3 rotateHelper = IdentityMatrix();
          // rotate the fish model  so that the head is pointing along the x axis
          rotateHelper.m[0] = 0;
          rotateHelper.m[1] = -1;
          rotateHelper.m[4] = 0;
          rotateHelper.m[5] = 1;
          rotateHelper.m[6] = -1;
          rotateHelper.m[8] = 0;
          copyMat3InMat4(m2w, mat3(m2w) * rotateHelper);*/

        // TODO move by 90 degress to the left
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
}