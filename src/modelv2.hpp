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
    MaterialTexture(Model *rawModel);
    MaterialTexture(std::string const &key, GLint texGPUSlot = 0, std::string const &path = "");

    GLint texID = -1;
    GLint texGPUSlot = -1;
    GLfloat factor = 1.0f;
  };

public:
  /*********CONSTRUCTORS*********/
  Modelv2(Model *raw, MaterialTexture tex = MaterialTexture(-1, -1))
      : raw{raw},
        m_texture{tex} {}
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

  Modelv2(Modelv2 const &other);
  Modelv2(Modelv2 &&other);

  Modelv2 &operator=(Modelv2 other);

  /************************/

  inline mat4 *matrix() { return m_matrix.get(); }
  inline mat4 const *matrix() const { return m_matrix.get(); }
  void matrix(mat4 matrix);

  // Return underlying Model
  inline Model *get() const { return raw; }
  inline void draw(GLuint program) const { DrawModel(raw, program); }

  void setLightProps(GLfloat Ka, GLfloat Kd, GLfloat Kspec, GLfloat alpha);
  inline MaterialLight *getLightProps() const { return lightProps.get(); }

  inline bool hasTexture() const { return m_texture.texID >= 0; }
  inline MaterialTexture texture() const { return hasTexture() ? m_texture : -1; }
  inline void texture(MaterialTexture texture) { m_texture = texture; }
  inline void textureFactor(GLfloat factor) { m_texture.factor = factor; }

private:
  Model *raw;
  std::unique_ptr<MaterialLight> lightProps{nullptr};
  MaterialTexture m_texture{-1, -1};
  std::unique_ptr<mat4> m_matrix{nullptr};
};