#include "fish.hpp"
#include "cmath"

namespace obj
{

    Fish::Fish(Model *m, vec3 pos, vec3 dir, vec3 up, vec3 size) : MoveableObject{m, pos, dir, up, size}
    {
        velocity = 0.05;
        // rotate the fish model  so that the head is pointing along the x axis
        additionalModelRotation.m[0] = 0;
        additionalModelRotation.m[1] = -1;
        additionalModelRotation.m[4] = 0;
        additionalModelRotation.m[5] = 1;
        additionalModelRotation.m[6] = -1;
        additionalModelRotation.m[8] = 0;

        MoveableObject::updateModelToWorldRotation(direction, up);
    };

    /*
    updatemovement vector - neuen übergebendann eine rotation in alle drei richtungen, wenn nötig
    */
    /*void Fish::updateModelToWorldRotation(vec3 newMovement, vec3 newUp)
    {
        MoveableObject::updateModelRotation(direction, up);
    }*/

    bool Fish::isCollision(const MoveableObject &other) const
    {
        vec3 thisToOther = other.position - this->position;
        float distance = Norm(thisToOther);
        return distance > longestDistanceFormCenter + other.longestDistanceFormCenter;
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
        position += direction * velocity;
        m2w.m[3] = position.x;
        m2w.m[7] = position.y;
        m2w.m[11] = position.z;
    }

    void Fish::adaptToTerrain(scn::Terrain &terrain)
    {
        float maxHeight = terrain.computeHeight(position.x, position.y);
        float highestPitch = -INFINITY;
        int indexOfSteepestPitch = 0;
        for (size_t i = 1; i < 5; i++)
        {
            vec3 futurePos = position + direction * velocity * i; // would touch it in the next five steps
            float futureHeight = terrain.computeHeight(futurePos.x, futurePos.y);
            maxHeight = std::max(maxHeight, futureHeight);
            if (highestPitch < futureHeight - position.y)
            {
                indexOfSteepestPitch = i;
                highestPitch = futureHeight - position.y;
            }
        }
        /*
        New movement= highestIndex as x and highestPitch as y -> daraus entsteht ein Bruch - Steigungsdreieck - rest scalen um auf die richtige Höhe zu kommen
        x und z gemeinsam bilden die x achse - hochskalieren
        */

        if (maxHeight > position.y + longestDistanceFormCenter)
        {
            //float xLengthToZLength = direction.x / direction.z; // sqrt(pow(direction.x, 2) + pow(direction.z, 2)); // correlate this to the

            direction.y = maxHeight;
            direction.x = indexOfSteepestPitch * direction.x;
            direction.z = indexOfSteepestPitch * direction.z;

            // movement must be adapted to prevent a collision increase y movement  - rotate the vector to match the new angle
            direction = normalize(direction);
            MoveableObject::updateModelToWorldRotation();
        }
// TODO check for an actual collision
        // TODO also check that they do not move out of the water on the top
    }
    void Fish::handleObjectCollision(MoveableObject *other) {}
}