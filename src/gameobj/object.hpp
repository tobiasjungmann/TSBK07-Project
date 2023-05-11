#pragma once
#include "VectorUtils4.h"
#include "scene/terrain.hpp"

namespace obj
{
    class MoveableObject
    {
    public:
        MoveableObject(Model *m, vec3 pos = vec3(0), vec3 dir = vec3(0), vec3 up = vec3(0, 1, 0), vec3 size = vec3(1, 1, 1)) : model(m), position(pos), direction(dir), up{up}, sizeInDirection{size}, longestDistanceFormCenter{Norm(size)} {};

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

        virtual void adaptToTerrain(scn::Terrain &terrain) = 0;
        virtual void handleObjectCollision(MoveableObject *other) = 0;

        const Model *getModel() const noexcept
        {
            return model;
        }

        mat4 getM2W() const
        {
            return m2w;
        }

        vec3 position = vec3(0, 0, 0);
        vec3 direction = vec3(0, 1, 0); // (fish) model is pointing upwards by default
        float velocity = 1;             // how fast is the object moving
        const float longestDistanceFormCenter;

    protected:
        mat4 m2w = IdentityMatrix();

        vec3 up = vec3(0, 0, -1);
        const Model *model;

        mat3 additionalModelRotation = IdentityMatrix();
        const vec3 sizeInDirection;

        /**
         * @brief Defined the size of the fish boundary (first line size in x, second size in y, third size in z)
         * // TODO update once the whole fish model is rotated
         */
        mat3 hitbox = IdentityMatrix(); // Deafult circle with length 1

        void copyMat3InMat4(mat4 &m2w, mat3 rotation)
        {
            for (size_t i = 0; i < 9; i++)
            {
                m2w.m[i + i / 3] = rotation.m[i]; // leave out the field in the last column
            }
        }

        vec3 getMatrixLine(mat3 &matrix, int i)
        {
            return vec3(matrix.m[i * 3], matrix.m[i * 3 + 1], matrix.m[i * 3 + 2]);
        }

        void updateModelToWorldRotation()
        {
            updateModelToWorldRotation(direction, up);
        }

        void updateModelToWorldRotation(vec3 newMovement)
        {
            updateModelToWorldRotation(newMovement, up);
        }

        void setVectorInLine(mat4 &matrix, vec3 input, int line)
        {
            matrix.m[0 + line * 4] = input.x;
            matrix.m[1 + line * 4] = input.y;
            matrix.m[2 + line * 4] = input.z;
        }

        void rotateHitbox(mat3 &hitbox, mat3 rotation)
        {
            mat3 baseHitbox = IdentityMatrix();
            baseHitbox.m[0] = sizeInDirection.x;
            baseHitbox.m[4] = sizeInDirection.y;
            baseHitbox.m[8] = sizeInDirection.z;
            hitbox = baseHitbox * rotation;
        }
        /**
         * @brief Alligns the x axis of the model with the movement vector and teh up vector
         * @param newMovement
         * @param newUp
         */
        void updateModelToWorldRotation(vec3 newMovement, vec3 newUp)
        {
            vec3 zaxis = CrossProduct(newUp, newMovement);
            vec3 yaxis = CrossProduct(newMovement, zaxis);

            setVectorInLine(m2w, newMovement, 0);
            setVectorInLine(m2w, yaxis, 1);
            setVectorInLine(m2w, zaxis, 2);

            direction = newMovement;
            up = newUp;

            mat3 combinedRotation = mat3(m2w) * additionalModelRotation;
            copyMat3InMat4(m2w, combinedRotation);
            rotateHitbox(hitbox, combinedRotation);

            mat4 scaling = IdentityMatrix();
            scaling.m[0] = sizeInDirection.x;
            scaling.m[5] = sizeInDirection.y;
            scaling.m[8] = sizeInDirection.z;
            m2w = m2w * scaling;
        }
    };

}