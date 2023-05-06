#include "fish.hpp"

namespace obj
{
    Fish::Fish(Model *m, vec3 pos, vec3 dir) : MoveableObject{m, pos, dir} {};

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