#include "modelv2.hpp"

void Modelv2::setLightProps(GLfloat Ka, GLfloat Kd, GLfloat Kspec, GLfloat alpha)
{
	if (!lightProps)
	{
    lightProps = std::make_unique<MaterialLight>(Ka, Kd, Kspec, alpha);
	}
  else{
    lightProps->alpha = alpha;
    lightProps->specularity = Kspec;
    lightProps->diffuseness = Kd;
    lightProps->Ka = Ka;
  }
}