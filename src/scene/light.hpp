#pragma once

#include "VectorUtils4.h"

namespace scn
{
    struct Light
    {
        vec3 intensity;
        vec3 direction;
        vec3 position;
        bool directional = false;
        GLfloat attenuConst;
        GLfloat attenuLinear;
        GLfloat attenuSquare;
        bool attenuate = false;
        GLfloat cutOff;
        GLfloat outerEdgeCutOff;
        bool spotlight = false;
    };
}