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

  struct MaterialTexture
  {
    MaterialTexture(GLint texID, GLint texGPUSlot = 0)
        : texID{texID}, texGPUSlot(texGPUSlot) {}

    MaterialTexture(Model *rawModel)
    {
      if (rawModel->material)
      {
        texID = rawModel->material->texRefID;
        texID = rawModel->material->texUnit;
      }
    }

    MaterialTexture(std::string const &key, GLint texGPUSlot = 0)
    {
      texID = rc::ResourceManager::get().getTexture(key);
      texGPUSlot = texGPUSlot;
    }

    GLint texID = -1;
    GLint texGPUSlot = -1;
  };

public:
  /*********CONSTRUCTORS*********/
  Modelv2(Model *raw, MaterialTexture tex = MaterialTexture(-1, -1))
      : raw{raw},
      m_texture{tex}
  {
  }
  Modelv2(Model *raw, MaterialLight &&props)
      : raw{raw},
        lightProps{std::make_unique<MaterialLight>(props)} {}

  Modelv2(std::string const &mdlKey,
          std::string const &mdlPath = "",
          std::string const &texKey = "",
          GLint texGPUSlot = 0,
          std::string const &texPath = "")
      : Modelv2(rc::ResourceManager::get().getModel(mdlKey, mdlPath),
                MaterialTexture(rc::ResourceManager::get().getTexture(texKey, texPath), texGPUSlot)) {}

  Modelv2(Modelv2 const &other)
      : raw{other.raw},
      m_texture{other.m_texture}
  {
    if (other.lightProps)
      lightProps = std::make_unique<MaterialLight>(*other.lightProps.get());
    else
      lightProps = nullptr;
    if (other.m_matrix)
      m_matrix = std::make_unique<mat4>(*other.m_matrix.get());
    else
      m_matrix = nullptr;
  }

  Modelv2(Modelv2 &&other)
      : raw{other.raw}
  {
    lightProps = std::move(other.lightProps);
    m_matrix = std::move(other.m_matrix);
    m_texture = other.m_texture;
  }

  Modelv2 &operator=(Modelv2 other)
  {
    // swap:
    std::swap(this->raw, other.raw);
    std::swap(this->lightProps, other.lightProps);
    std::swap(this->m_matrix, other.m_matrix);
    std::swap(this->m_texture, other.m_texture);
    return *this;
  }

  /************************/

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

  inline void draw(GLuint program) const
  {
    DrawModel(raw, program);
  }

  // inline void init(GLuint program, const char *vertexVariableName, const char *normalVariableName)
  // {
  //   InitModel(raw, program, vertexVariableName, normalVariableName, NULL, -1, -1);
  // }
  // inline void init(GLuint program, const char *vertexVariableName, const char *normalVariableName, const char *texCoordVariableName, MaterialTexture texInfo)
  // {
    
  // }

  void setLightProps(GLfloat Ka, GLfloat Kd, GLfloat Kspec, GLfloat alpha);

  inline MaterialLight *getLightProps() const { return lightProps.get(); }

  // Return underlying Model
  inline Model *get() const { return raw; }

  inline bool hasTexture() const { return m_texture.texID >= 0; }
  inline MaterialTexture texture() const { return (raw->material) ? raw->material->texRefID : -1; }
  inline void texture(MaterialTexture texture) { m_texture = texture; }

private:
  Model *raw;
  std::unique_ptr<MaterialLight> lightProps{nullptr};
  MaterialTexture m_texture{-1, -1};
  std::unique_ptr<mat4> m_matrix{nullptr};
};