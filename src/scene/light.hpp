#pragma once

#include "VectorUtils4.h"

namespace scn
{
    struct Light
    {
        vec3 intensity;
        vec3 dirPos;
        bool directional;
    };
}