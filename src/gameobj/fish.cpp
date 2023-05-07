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

    Fish::Fish(Model *m, vec3 pos, vec3 dir) : MoveableObject{m, pos}
    {
        updateMovementVector(normalize(dir), vec3(0, 1, 0));
    };

    /*
    updatemovement vector - neuen übergebendann eine rotation in alle drei richtungen, wenn nötig
    */
    void Fish::updateMovementVector(vec3 newMovement, vec3 newUp)
    {

        float angle = dot(newMovement, movementDirection);
        float rotation_factor = M_1_PI * 5;
        mat3 rotateZ = IdentityMatrix();
        mat3 rotateX = IdentityMatrix();

        // rotate the fish model to point in the initial direction vector
        rotateZ.m[0] = cos(rotation_factor);
        rotateZ.m[2] = sin(rotation_factor);
        rotateZ.m[6] = -sin(rotation_factor);
        rotateZ.m[8] = cos(rotation_factor);

        rotateX.m[0] = cos(rotation_factor);
        rotateX.m[1] = -sin(rotation_factor);
        rotateX.m[3] = sin(rotation_factor);
        rotateX.m[4] = cos(rotation_factor);

        rotateZ = rotateX * rotateZ;
        // copyMat3InMat4(m2w,rotateZ);
        //  mat4 lookatRotate=lookAt(position,position+newMovement,newUp);//*rotateZ;
        //  mat3 lookatRotatesmall=mat3(lookAt(position,position+newMovement,newUp));//*
        //  copyMat3InMat4(m2w,lookatRotate);
        // mat4 rotateHelper=IdentityMatrix();
        /* rotateHelper.m[0]=newMovement.x;
         rotateHelper.m[4]=newMovement.y;
         rotateHelper.m[8]=newMovement.z;*/
        /*   rotateHelper.m[0]=newMovement.x;
           rotateHelper.m[5]=newMovement.y;
           rotateHelper.m[10]=newMovement.z;
           m2w=IdentityMatrix();
           m2w=m2w*rotateHelper;*/
        m2w.m[0] = newMovement.x;
        m2w.m[1] = newMovement.y;
        m2w.m[2] = newMovement.z;

        vec3 zaxis = CrossProduct(vec3(0, 1, 0), newMovement);
        vec3 yaxis = CrossProduct(newMovement, zaxis);
        m2w.m[4] = yaxis.x;
        m2w.m[5] = yaxis.y;
        m2w.m[6] = yaxis.z;

        m2w.m[8] = zaxis.x;
        m2w.m[9] = zaxis.y;
        m2w.m[10] = zaxis.z;

        copyMat3InMat4(m2w,mat3(m2w)*rotateZ);
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