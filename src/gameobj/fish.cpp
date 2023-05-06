#include "fish.hpp"

namespace obj
{
    Fish::Fish(Model *m, vec3 pos, vec3 dir): MoveableObject{m, pos, dir}{
    };

    bool Fish::isCollision(const MoveableObject &other) const
    {
        return false;
    }

    void Fish::collide(vec3 position, vec3 normalToCollisionPoint)
    {
        ;
    }

    void Fish::moveSingleStep()
    {
        //m2w=;
    }

    mat4 Fish::getM2W()
    {
        return m2w;
    }
}