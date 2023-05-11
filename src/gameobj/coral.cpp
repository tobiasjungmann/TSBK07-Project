#include "coral.hpp"
#include "cmath"

namespace obj
{
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
    {
        additionalModelRotation = zRotation();
        MoveableObject::updateModelToWorldRotation(direction, up);
        position=position-normalize(direction);     // move the stemp tin the ground
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

    void Coral::adaptToTerrain(scn::Terrain &terrain)
    {
        // Coral cannot move - nothing changes
    }
    void Coral::handleObjectCollision(MoveableObject *other)
    {
        // Coral cannot move - nothing changes
        // Everything goes trough an coral nothing changes
    }
}