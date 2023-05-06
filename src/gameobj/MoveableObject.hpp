#pragma once
#include "VectorUtils4.h"

namespace obj
{
    class MoveableObject
    {
    public:
        MoveableObject(Model *m, vec3 pos = vec3(0), vec3 dir = vec3(0)) : model(m), position(pos), movementDirection(dir){
        };

        virtual void moveSingleStep() = 0;
        virtual mat4 getM2W() = 0;
        virtual bool isCollision(const MoveableObject &other) const = 0;
        virtual void collide(vec3 position, vec3 normalToCollisionPoint) = 0;
        const Model *getModel() const noexcept
        {
            return model;
        }

    protected:
        mat4 m2w = IdentityMatrix();
        vec3 position = vec3(0, 0, 0);
        vec3 movementDirection = vec3(1, 0, 0);
        const Model *model;
    };
}