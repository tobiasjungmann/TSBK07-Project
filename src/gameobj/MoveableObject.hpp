#pragma once
#include "VectorUtils4.h"

namespace obj
{
   class MoveableObject
    {
    public:
        virtual bool isCollision(const MoveableObject &other) const=0;
        virtual void collide(vec3 position, vec3 normalToCollisionPoint)=0;
        MoveableObject(Model *m, vec3 pos = vec3(0), vec3 dir = vec3(0)):model(m),position(pos),movementDirection(dir){};

    protected:
        vec3 position;
        vec3 movementDirection;
        const Model *model;
    };
}