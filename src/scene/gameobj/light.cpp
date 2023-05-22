#include "light.hpp"

namespace scn
{
  Light::Light(vec3 color, vec3 posOrDir, bool directional) : color{color}, directional{directional}
  {
    if (directional)
      direction = posOrDir;
    else
      position = posOrDir;
  }


  void Light::setAttenuation(GLfloat konst, GLfloat linear, GLfloat square)
  {
    attenuConst = konst;
    attenuLinear = linear;
    attenuSquare = square;
    attenuate = true;
  }

  void Light::setSpotlight(GLfloat innerEdgeDegrees, GLfloat outerEdgeDegrees)
  {
    spotlight = true;
    cutOff = cos(innerEdgeDegrees * (M_PI / 180));
    outerEdgeCutOff = cos((outerEdgeDegrees * (M_PI / 180)));
  }

  void Light::setCoefficients(GLfloat ambient, GLfloat diffuse, GLfloat specular, GLfloat power)
  {
    ambientK = CLAMP(ambient, 0, 1.0);
    diffuseK = CLAMP(diffuse, 0, 1.0);
    specularK = CLAMP(specular, 0, 1.0);
    powerK = power;
  }

  // empty parameter = detach
  void Light::attachToCamera(scn::Camera &camera)
  {
    position.inView = true;
    camera.attachDirection(direction);
    camera.attachPosition(position);
  }

  void Light::detachOfCamera(scn::Camera &camera)
  {
    position.inView = false;
    camera.attachDirection(direction);
    camera.attachPosition(position);
  }
}