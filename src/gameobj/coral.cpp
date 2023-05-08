#include "coral.hpp"
#include "cmath"

namespace obj
{

    Coral::Coral(Model *m, vec3 pos, vec3 dir, vec3 up) : MoveableObject{m, pos, dir, up} {
        // TODO move to surface
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