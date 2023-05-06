#include "fish.hpp"

namespace obj
{
    Fish::Fish(Model *m, vec3 pos, vec3 dir) : MoveableObject{m, pos, dir} {};

    bool Fish::isCollision(const MoveableObject &other) const
    {
        return false;
    }
    void copyMat3InM2W(mat4& m2w,mat3 rotation){
        for (size_t i = 0; i < 9; i++)
        {
         m2w.m[i+i/3] = rotation.m[i];      // leave out the field in hte last column
        }
    }

    void Fish::collide(vec3 position, vec3 normalToCollisionPoint)
    {
        //TODO add reflection on normal
        //TODO also alter the up vector for non 2D reflections
        copyMat3InM2W(m2w,lookAt(position, position + movementDirection, up));
        
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