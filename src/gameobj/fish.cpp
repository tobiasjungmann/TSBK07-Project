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

    Fish::Fish(Model *m, vec3 pos, vec3 dir) : MoveableObject{m, pos}
    {
        updateMovementVector(normalize(dir), vec3(0, 1, 0));
    };

    void setVectorInLine(mat4 &matrix, vec3 input, int line)
    {
        matrix.m[0 + line * 4] = input.x;
        matrix.m[1 + line * 4] = input.y;
        matrix.m[2 + line * 4] = input.z;
    }

    /*
    updatemovement vector - neuen übergebendann eine rotation in alle drei richtungen, wenn nötig
    */
    void Fish::updateMovementVector(vec3 newMovement, vec3 newUp)
    {
        float rotation_factor = M_1_PI * 5;
        mat3 rotateHelper = IdentityMatrix();
        mat3 rotateX = IdentityMatrix();

        // rotate the fish model  so that the head is pointing along the x axis
        rotateHelper.m[0] = cos(rotation_factor);
        rotateHelper.m[2] = sin(rotation_factor);
        rotateHelper.m[6] = -sin(rotation_factor);
        rotateHelper.m[8] = cos(rotation_factor);

        rotateX.m[0] = cos(rotation_factor);
        rotateX.m[1] = -sin(rotation_factor);
        rotateX.m[3] = sin(rotation_factor);
        rotateX.m[4] = cos(rotation_factor);

        rotateHelper = rotateX * rotateHelper;

        vec3 zaxis = CrossProduct(vec3(0, 1, 0), newMovement);
        vec3 yaxis = CrossProduct(newMovement, zaxis);
       
        setVectorInLine(m2w, newMovement, 0);
        setVectorInLine(m2w, yaxis, 1);
        setVectorInLine(m2w, zaxis, 2);

        copyMat3InMat4(m2w, mat3(m2w) * rotateHelper);
        movementDirection = newMovement;
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
        position += movementDirection * 0.05;
        m2w.m[3] = position.x;
        m2w.m[7] = position.y;
        m2w.m[11] = position.z;
    }

    mat4 Fish::getM2W()
    {
        return m2w;
    }
}