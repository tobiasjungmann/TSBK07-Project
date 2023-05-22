#include "fish.hpp"
#include <cmath>
#include "../terrain.hpp"
#include "gameobj.hpp"
#include "../scene.hpp"
#include "../constants.hpp"

namespace obj
{

    Fish::Fish(Modelv2 mdl,
               vec3 pos,
               vec3 dir,
               float speed,
               vec3 up,
               vec3 size) : CollidingObject{mdl, pos, dir, up, size},
                            m_speed{speed}
    {
        if (m_model.matrix() == nullptr)
            m_model.matrix(IdentityMatrix());
        m_longestDistanceFromCenter = 8;
        // rotate the fish model  so that the head is pointing along the x axis
        updateModelToWorldRotation();
    };

    float randomMovementInDir()
    {
        float r = (float)(rand() / (float)RAND_MAX);
        if (r < 0.2)
        {
            float current=0.1;
            if (r < 0.1)
            {
                current = current * (-1);
            }
            return current ;
        }
        return 0;
    }

    void Fish::moveSingleStep()
    {
        if ((float)(rand() / (float)RAND_MAX) < 0.1)
        {
            m_direction().y += randomMovementInDir() * m_speed;
            m_direction().x += randomMovementInDir() * m_speed;
            m_direction().z += randomMovementInDir() * m_speed;
            m_direction() = normalize(m_direction());
            updateModelToWorldRotation();
        }
        m_position += m_direction * m_speed;
        model().matrix()->m[3] = m_position().x;
        model().matrix()->m[7] = m_position().y;
        model().matrix()->m[11] = m_position().z;
    }

    void Fish::updateModelToWorldRotation()
    {
        CollidingObject::updateModelToWorldRotation();
        model().matrix()->m[2] *= -1;
        model().matrix()->m[5] *= -1; // FIXME check here that it was like that
    }

    void Fish::update(scn::Scene &scene)
    {
    }
    void Fish::update(scn::Scene &scene, size_t index)
    {
        rotateHitbox(mat3(*model().matrix()) * orientationMtx());

        if (scene.terrain)
        {
            adaptToTerrain(*scene.terrain);
            if (m_position().x != (*scene.terrain).nextInsideFieldWidth(m_position().x, 1))
            { // change direction only once until margin is left.
                if ((m_position().x < (*scene.terrain).height() / 2 && m_direction().x < 0) || (m_position().x > (*scene.terrain).height() / 2 && m_direction().x > 0))
                {
                    m_direction().x = -m_direction().x;
                }
            }
            if (m_position().z != (*scene.terrain).nextInsideFieldWidth(m_position().z, 10))
            {
                if ((m_position().z < (*scene.terrain).width() / 2 && m_direction().z < 0) || (m_position().z > (*scene.terrain).width() / 2 && m_direction().z > 0))
                {
                    m_direction().z = -m_direction().z;
                }
            }
        }

        for (size_t u = 0; u < index; u++)
        {
            auto collideObj{dynamic_cast<obj::CollidingObject *>(scene.getObj(u))};
            if (collideObj)
                handleObjectCollision(collideObj);
        }
        moveSingleStep();
    }

    void Fish::adaptToTerrain(scn::Terrain &terrain)
    {
        float maxHeightDifference = -INFINITY;
        float maxHeightDifferenceToStart = 0;
        int indexOfSteepestPitch = 0;

        for (size_t i = 1; i < 5; i++)
        {

            vec3 futurePos = m_position + m_direction * m_speed * i * 10; // would touch it in the next five steps

            float futureHeight = terrain.computeHeight(futurePos.x, futurePos.z);

            if (maxHeightDifference < futureHeight - futurePos.y)
            {
                maxHeightDifference = futureHeight - futurePos.y;
                indexOfSteepestPitch = i;
                maxHeightDifferenceToStart = futureHeight - m_position().y;
            }
        }

        if (maxHeightDifference > -m_longestDistanceFromCenter + 1)
        {
            m_direction().y = std::abs(maxHeightDifferenceToStart + m_longestDistanceFromCenter) / indexOfSteepestPitch;
            m_direction().x = indexOfSteepestPitch * m_direction().x;
            m_direction().z = indexOfSteepestPitch * m_direction().z;

            // movement must be adapted to prevent a collision increase y movement  - rotate the vector to match the new angle
            m_direction = normalize(m_direction);
            updateModelToWorldRotation();
        }
        else if (maxHeightDifference < -m_longestDistanceFromCenter && m_direction().y > 0)
        {

            m_direction().y = 0;
            if (Norm(m_direction) == 0)
            {
                m_direction = vec3(1, 0, 0);
            }
            m_direction = normalize(m_direction);
            updateModelToWorldRotation();
        }
        // TODO also check that they do not move out of the water on the top
    }
    void Fish::handleObjectCollision(CollidingObject *other)
    {
        // TODO refine for the actual position
        vec3 thisToOther = other->position() - this->m_position;
        float distance = Norm(thisToOther);
        if (distance <= Norm(this->m_size) + Norm(other->size()))
        {
            vec3 n = normalize(thisToOther);
            other->reflectOnVector(n * (-1));
            reflectOnVector(n);
        }
    }
}