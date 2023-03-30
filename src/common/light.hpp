#pragma once

#include "VectorUtils4.h"

namespace scn
{
    struct Light
    {
        vec4 intensity;
        union
        {
            vec3 direction, position;
        };
        const bool directional;

        constexpr Light(vec4 intensity, vec3 positionDirection, bool isDirectional) 
            : intensity{intensity}, direction{positionDirection}, directional{isDirectional} {}
        constexpr Light(vec3 intensity, vec3 positionDirection, bool isDirectional) 
            : Light({intensity, 1.0f}, positionDirection, isDirectional) {}
    };
}