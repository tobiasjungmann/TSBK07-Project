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
    Fish::Fish(Model *m, vec3 pos, vec3 dir, vec3 up, float scalingFactor) : MoveableObject{m, pos, dir, up, scalingFactor}
    {
         // rotate the fish model  so that the head is pointing along the x axis
        additionalModelRotation.m[0] = 0;
        additionalModelRotation.m[1] = -1;
        additionalModelRotation.m[4] = 0;
        additionalModelRotation.m[5] = 1;
        additionalModelRotation.m[6] = -1;
        additionalModelRotation.m[8] = 0;

        updateModelRotation(dir, up);
    };



    /*
    updatemovement vector - neuen übergebendann eine rotation in alle drei richtungen, wenn nötig
    */
    /*void Fish::updateModelRotation(vec3 newMovement, vec3 newUp)
    {
        MoveableObject::updateModelRotation(direction, up);
    }*/

    bool Fish::isCollision(const MoveableObject &other) const
    {
        // vec3 normalDir=normalize(direction);
        vec3 thisToOther = other.position - this->position;
        float distance = Norm(thisToOther);
        return distance>longestDistanceFormCenter + other.longestDistanceFormCenter;
        
        //float sizeInDirThis = normalize(thisToOther) * (hitbox);

        // TODO also for the second direction then compare
        // if (/* condition */)
        ///{
        /* code */
        //}

        //   double velocity, b, c;
        // double temp, t1, t2;
        //  TODO do not compute if they are too far away
        /*
        movement normalisieren - jede der drei Seitenangaben mit dem entsprechenden vector factor multiplizieren - länge bekannt

        */

        // midnight formula
        /*	velocity = sqr(this->direction.x - other.direction.x) +  sqr(this->direction.y- other.direction.y)+ sqr(this->direction.z - other.direction.z);
            b = 2 * ((this->position.x - other.position.x) * (this->direction.x - other.direction.x) + (this->direction.y - other.position.y) * (this->direction.y - other.direction.y));
            c = sqr(this->position.x - other.position.x) + sqr(this->position.y - other.position.y) - 4 * 1 * 1;

            if (velocity != 0.0)
            {
                // final form of the midnight formula
                temp = sqr(b) - 4 * velocity * c;
                if (temp >= 0)
                {
                    temp = sqrt(temp);
                    // compute both x values
                    t1 = (-b + temp) / (2 * velocity);
                    t2 = (-b - temp) / (2 * velocity);

                    if (t1 > t2)
                    {
                        temp = t1;
                        t1 = t2;
                        t2 = temp;
                    }
                    if ((t1 >= 0) & (t1 <= 1))
                        return t1;
                    else if ((t2 >= 0) & (t2 <= 1))
                        return t2;
                }
            }
            return -1;*/
    }

    void Fish::collide(vec3 position, vec3 normalToCollisionPoint)
    {
        /*   vec3 this_to_other = other.position - position;
           if (vectorLength(this_to_other) > radius + other.radius)
           {
               return false; // they cannot collide.
           }
           //printf("Collision detected: %f < %f\n", vectorLength(this_to_other), radius + other.radius);
           return true;*/
        // TODO add reflection on normal
        // TODO also alter the up vector for non 2D reflections
        // copyMat3InM2W(m2w,lookAt(position, position + movementDirection, up));

        // Same prinziple as in parallel computing - is there a collision in one of the nex x linear steps - if so adapt the movement
    }

    void Fish::moveSingleStep()
    {
        position += direction * 0.05;
        m2w.m[3] = position.x;
        m2w.m[7] = position.y;
        m2w.m[11] = position.z;
    }
}