#pragma once

#include "VectorUtils4.h"
#include "gameobj.hpp"
#include "../scene/camera.hpp"
#include <cmath>

namespace scn
{
  struct Light
  {
    Light(vec3 color, vec3 posOrDir, bool directional);

    void setAttenuation(GLfloat konst, GLfloat linear, GLfloat square);
    void setSpotlight(GLfloat innerEdgeDegrees, GLfloat outerEdgeDegrees);
    void setCoefficients(GLfloat ambient, GLfloat diffuse, GLfloat specular);
    void attachToCamera(scn::Camera &camera);
    void detachOfCamera(scn::Camera &camera);

    vec3 color;
    obj::AnchoredProp<vec3> direction{vec3(0)};
    obj::prop::Position position;

    // coefficients for different effect
    GLfloat ambientK = 0.0;
    GLfloat diffuseK = 1.0;
    GLfloat specularK = 1.0;

    // attenuation over distance
    GLfloat attenuConst = 1.0;
    GLfloat attenuLinear = 0.0;
    GLfloat attenuSquare = 0.0;
    // spotlight
    GLfloat cutOff = 1.0;
    GLfloat outerEdgeCutOff = 1.0;
    bool directional = false;
    bool attenuate = false;
    bool spotlight = false;
  };
}