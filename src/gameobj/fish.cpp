#include "fish.hpp"
#include "cmath"

namespace obj
{

    Fish::Fish(Model *m, vec3 pos, vec3 dir, vec3 up, vec3 size) : MoveableObject{m, pos, dir, up, size}
    {
        speed = 0.05;
        longestDistanceFormCenter = 8;

        // rotate the fish model  so that the head is pointing along the x axis
        additionalModelRotation.m[0] = 0;
        additionalModelRotation.m[1] = -1;
        additionalModelRotation.m[4] = 0;
        additionalModelRotation.m[5] = 1;
        additionalModelRotation.m[6] = -1;
        additionalModelRotation.m[8] = 0;

        updateModelToWorldRotation();
    };

    bool Fish::isCollision(const MoveableObject &other) const
    {
        // TODO refine for the actual position
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
        position += direction * speed;
        m2w.m[3] = position.x;
        m2w.m[7] = position.y;
        m2w.m[11] = position.z;
        // std::cout << "Position: " << position.x << " " << position.y << " " << position.z;
        // std::cout << "  Direction: " << direction.x << " " << direction.y << " " << direction.z << std::endl;
    }

    void Fish::updateModelToWorldRotation()
    {
        MoveableObject::updateModelToWorldRotation();
        m2w.m[2] = -m2w.m[2];
        m2w.m[5] = -m2w.m[5];
    }

    void Fish::adaptToTerrain(scn::Terrain &terrain)
    {
        float maxHeightDifference = -INFINITY;
        float maxHeightDifferenceToStart = 0;
        float highestPitch = -INFINITY;
        int indexOfSteepestPitch = 0;

        for (size_t i = 1; i < 5; i++)
        {
            vec3 futurePos = position + direction * speed * i * 10; // would touch it in the next five steps
            float futureHeight = terrain.computeHeight(futurePos.x, futurePos.z);

            if (maxHeightDifference, futureHeight - futurePos.y)
            {
                maxHeightDifference = futureHeight - futurePos.y;
                indexOfSteepestPitch = i;
                maxHeightDifferenceToStart = futureHeight - position.y;
            }
        }

        if (maxHeightDifference > -longestDistanceFormCenter + 1)
        {
            direction.y = std::abs(maxHeightDifferenceToStart + longestDistanceFormCenter) / indexOfSteepestPitch;
            direction.x = indexOfSteepestPitch * direction.x;
            direction.z = indexOfSteepestPitch * direction.z;

            // movement must be adapted to prevent a collision increase y movement  - rotate the vector to match the new angle
            direction = normalize(direction);
            updateModelToWorldRotation();
        }
        else if (maxHeightDifference < -longestDistanceFormCenter && direction.y > 0)
        {

            direction.y = 0;

            direction = normalize(direction);
            updateModelToWorldRotation();
        }
        // TODO also check that they do not move out of the water on the top
    }
    void Fish::handleObjectCollision(MoveableObject *other) {}
}