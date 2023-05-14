#include "modelv2.hpp"

Modelv2::MaterialTexture::MaterialTexture(Model *rawModel)
{
  if (rawModel->material)
  {
    texID = rawModel->material->texRefID;
    texID = rawModel->material->texUnit;
  }
}

Modelv2::MaterialTexture::MaterialTexture(std::string const &key, GLint texGPUSlot, std::string const& path)
{
  texID = rc::ResourceManager::get().getTexture(key, path);
  this->texGPUSlot = texGPUSlot;
}

void Modelv2::setLightProps(GLfloat Ka, GLfloat Kd, GLfloat Kspec, GLfloat alpha)
{
  if (!lightProps)
  {
    lightProps = std::make_unique<MaterialLight>(Ka, Kd, Kspec, alpha);
  }
  else
  {
    lightProps->alpha = alpha;
    lightProps->specularity = Kspec;
    lightProps->diffuseness = Kd;
    lightProps->Ka = Ka;
  }
}

Modelv2::Modelv2(Modelv2 const &other)
    : raw{other.raw},
      m_texture{other.m_texture}
{
  if (other.lightProps)
    lightProps = std::make_unique<MaterialLight>(*other.lightProps.get());
  else
    lightProps = nullptr;
  if (other.m_matrix.get())
    m_matrix = std::make_unique<mat4>(*other.m_matrix.get());
  else
    m_matrix = nullptr;
}

Modelv2::Modelv2(Modelv2 &&other)
    : raw{other.raw}
{
  lightProps = std::move(other.lightProps);
  m_matrix = std::move(other.m_matrix);
  m_texture = other.m_texture;
}

Modelv2 &Modelv2::operator=(Modelv2 other)
{
  // swap:
  std::swap(this->raw, other.raw);
  std::swap(this->lightProps, other.lightProps);
  std::swap(this->m_matrix, other.m_matrix);
  std::swap(this->m_texture, other.m_texture);
  return *this;
}

void Modelv2::matrix(mat4 matrix)
{
  if (m_matrix == nullptr)
    m_matrix = std::make_unique<mat4>();
  *m_matrix = matrix;
}