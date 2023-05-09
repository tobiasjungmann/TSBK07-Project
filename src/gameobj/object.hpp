#pragma once
#include "VectorUtils4.h"

namespace obj
{
    class MoveableObject
    {
    public:
        MoveableObject(Model *m, vec3 pos = vec3(0), vec3 dir = vec3(0), vec3 up = vec3(0, 1, 0)) : model(m), position(pos), direction(dir), up{up} {
            updateM2W(normalize(dir), up);
        };

        virtual void moveSingleStep() = 0;

/**
 * @brief Checks if this model and the other are overlapping at any point
 * 
 * @param other model to have contact
 * @return true boundary boxes of the models are overlapping
 * @return false no overlap
 */
        virtual bool isCollision(const MoveableObject &other) const = 0;

        /**
         * @brief Change the movement of the model after it collided on the point given by the position with the given normal
         * 
         * @param position 
         * @param normalToCollisionPoint 
         */
        virtual void collide(vec3 position, vec3 normalToCollisionPoint) = 0;

        const Model *getModel() const noexcept
        {
            return model;
        }

        mat4 getM2W() const
        {
            return m2w;
        }

    protected:
        mat4 m2w = IdentityMatrix();
        vec3 position = vec3(0, 0, 0);
        vec3 direction = vec3(0, 1, 0); // model is pointing upwards by default
        vec3 up = vec3(0, 0, -1);
        const Model *model;

        void updateM2W(vec3 newMovement)
        {
            updateM2W(newMovement,up);
        }

    void setVectorInLine(mat4 &matrix, vec3 input, int line)
    {
        matrix.m[0 + line * 4] = input.x;
        matrix.m[1 + line * 4] = input.y;
        matrix.m[2 + line * 4] = input.z;
    }

/**
 * @brief Alligns the x axis of the model with the movement vector and teh up vector
 * 
 * @param newMovement 
 * @param newUp 
 */
        void updateM2W(vec3 newMovement, vec3 newUp)
        {
            vec3 zaxis = CrossProduct(newUp, newMovement);
            vec3 yaxis = CrossProduct(newMovement, zaxis);

            setVectorInLine(m2w, newMovement, 0);
            setVectorInLine(m2w, yaxis, 1);
            setVectorInLine(m2w, zaxis, 2);

            direction=newMovement;
            up=newUp;
        }
    };


}