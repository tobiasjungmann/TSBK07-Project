#pragma once

#include "../common/LittleOBJLoader.h"
#include "../resource_manager.hpp"
#include <memory>

class Modelv2
{
public:
  struct MaterialLight
  {
    MaterialLight(GLfloat ambientCoeff, GLfloat diffuseness, GLfloat specularity, GLfloat specExponent)
        : Ka{ambientCoeff},
          Kd{diffuseness},
          Ks{specularity},
          alpha{specExponent} {}

    union
    {
      GLfloat Ka = 1.0;
      GLfloat ambientCoeff;
    };
    union
    {
      GLfloat Kd = 1.0;
      GLfloat diffuseness;
    };
    union
    {
      GLfloat Ks = 1.0;
      GLfloat specularity;
    };

    union
    {
      GLfloat specularExponent, alpha = 1.0;
    };
  };

public:
  // Modelv2() : raw{nullptr}, lightProps{nullptr} {}
  Modelv2(Model *raw) : raw{raw}, lightProps{nullptr} {}
  Modelv2(Model *raw, MaterialLight &&props) : raw{raw}, lightProps{std::make_unique<MaterialLight>(props)} {}
  Modelv2(std::string const &key, std::string const &path = "") : Modelv2(ResourceManager::get().getModel(key, path)) {}
  Modelv2(Modelv2 const &other) : raw{other.raw}, lightProps{nullptr}
  {
    if (other.lightProps)
    {
      lightProps = std::make_unique<MaterialLight>(*other.lightProps.get());
    }
  }
  Modelv2 &operator=(Modelv2 other)
  {
    // swap:
    std::swap(this->raw, other.raw);
    std::swap(this->lightProps, other.lightProps);
    return *this;
  }

  inline mat4 *matrix()
  {
    return m_matrix.get();
  }

  inline mat4 const *matrix() const
  {
    return m_matrix.get();
  }

  inline void matrix(mat4 matrix)
  {
    if (m_matrix == nullptr)
      m_matrix = std::make_unique<mat4>();
    *m_matrix = matrix;
  }

  inline void draw(GLuint program)
  {
    DrawModel(raw, program);
  }
  inline void init(GLuint program, const char *vertexVariableName, const char *normalVariableName, const char *texCoordVariableName)
  {
    init(program, vertexVariableName, normalVariableName, texCoordVariableName, -1, -1);
  }

  inline void init(GLuint program, const char *vertexVariableName, const char *normalVariableName, const char *texCoordVariableName, GLint texUnit, GLint texReferenceID)
  {
    InitModel(raw, program, vertexVariableName, normalVariableName, texCoordVariableName, texUnit, texReferenceID);
  }

  void setLightProps(GLfloat Ka, GLfloat Kd, GLfloat Kspec, GLfloat alpha);

  inline MaterialLight *getLightProps() const { return lightProps.get(); }

  // Return underlying Model
  inline Model *get() const { return raw; }

private:
  Model *raw;
  std::unique_ptr<MaterialLight> lightProps;
  std::unique_ptr<mat4> m_matrix;
};