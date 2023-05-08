#pragma once

#include "VectorUtils4.h"

namespace scn
{
  struct Light
  {
    Light(vec3 color, vec3 posOrDir, bool directional) : color{color}, directional{directional} {
      if (directional) direction = posOrDir;
      else position = posOrDir;
    }

    inline void setAttenuation(GLfloat konst, GLfloat linear, GLfloat square)
    {
      attenuConst = konst;
      attenuLinear = linear;
      attenuSquare = square;
      attenuate = true;
    }

    inline void setSpotlight(GLfloat innerEdge, GLfloat outerEdge) {
      spotlight = true;
      cutOff = innerEdge;
      outerEdge = outerEdgeCutOff;
    }

    inline void setCoefficients(GLfloat ambient, GLfloat diffuse, GLfloat specular) {
      ambientK = CLAMP(ambient, 0, 1.0);
      diffuseK = CLAMP(diffuse, 0, 1.0);
      specularK = CLAMP(specular, 0, 1.0);
    }

    vec3 color;
    vec3 direction;
    vec3 position;

    // coefficients for different effect
    GLfloat ambientK = 0.0;
    GLfloat diffuseK = 1.0;
    GLfloat specularK = 1.0;

    // attenuation over distance
    GLfloat attenuConst;
    GLfloat attenuLinear;
    GLfloat attenuSquare;
    // spotlight
    GLfloat cutOff;
    GLfloat outerEdgeCutOff;
    bool directional = false;
    bool attenuate = false;
    bool spotlight = false;
  };
}