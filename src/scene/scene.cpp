#include "scene.hpp"

#include "debugging.h"

#include <unordered_set>
#include <stdexcept>
#include <utility>
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include "types_properties.hpp"

using std::get;
using std::size_t;

namespace _helpers
{
  using namespace scn;

  mat4 buildPreProj(Camera const& camera, mat4 const* m2w) {
      return camera.matrix() * ( m2w ? *m2w : IdentityMatrix());
  }

  void uploadModelData(Scene const& scene, Modelv2 model, bool alsoSynthesisPreProj)
  {
    auto m2w = model.matrix();
    auto const& shader = scene.shader;
    glBindVertexArray(model.get()->vao);
    if (alsoSynthesisPreProj)
    {
      shader->uploadMatrix(Shader::Matrices::preProj, buildPreProj(scene.camera, m2w));
    }

    if (m2w != nullptr)
      shader->uploadMatrix(Shader::Matrices::m2w, *m2w);

    // upload material of
    if (shader->hasLighting())
    {
      shader->uploadModelLightProps(model.getLightProps());
    }

    if (shader->hasTexturing())
      shader->uploadTexture(model.get());
    glDrawElements(GL_TRIANGLES, model.get()->numIndices, GL_UNSIGNED_INT, 0L);
  }
}
namespace scn
{

  Scene::Scene()
      : camera{0, 0, 0},
        projectionMatrix{IdentityMatrix()}
  {
    invalid = true;
  }

  Scene::Scene(std::unique_ptr<SceneShader> shader,
               const Camera &camera,
               mat4 projectionMatrix,
               std::unique_ptr<Terrain> terrain,
               std::unique_ptr<Skybox> skybox)
      : camera{camera},
        projectionMatrix{projectionMatrix},
        m_shader{std::move(shader)},
        terrain{std::move(terrain)},
        skybox{std::move(skybox)}
  {
    invalid = false;
  }

  Scene &Scene::operator=(Scene &&moved) noexcept
  {
    if (moved.invalid)
      throw std::invalid_argument("Tried to assign an invalid Scene.");
    m_shader = std::move(moved.m_shader);
    projectionMatrix = moved.projectionMatrix;
    camera = moved.camera;
    invalid = moved.invalid;
    skybox = std::move(moved.skybox);
    terrain = std::move(moved.terrain); // BUG write move operator for Terrain
    m_models = std::move(moved.m_models);

    dbg::if_dlevel<dbg::Level::VERBOSE>([]()
                                        { std::cerr << "move assigment" << std::endl; });

    return *this;
  }

  void Scene::popModel()
  {
    m_models.pop_back();
  }

  void Scene::pushModel(Modelv2 newMdl)
  {
    m_models.emplace_back(newMdl);
  }

  void Scene::pushModel(Modelv2 newMdl, mat4 m2wMtx)
  {
    newMdl.matrix(m2wMtx);
    pushModel(newMdl);
  }

  void Scene::updateModelM2W(long modelIndex, mat4 update)
  {
    if (modelIndex < 0)
      modelIndex += m_models.size();
    m_models[modelIndex].matrix(update);
  }

  void Scene::addLightSource(const Light &light)
  {
    if (not shader->hasLighting() && !ENV_NOTHROW)
      throw std::logic_error("Attempt to add light sources without prior initialization of the lighting engine");
    m_lights.push_back(light);
  }

  void Scene::removeLightSource(long index)
  {
    if (not shader->hasLighting() && !ENV_NOTHROW)
      throw std::logic_error("Attempt to add light sources without prior initialization of the lighting engine");

    if (index < 0)
      index += m_lights.size();

    m_lights.erase(std::begin(m_lights) + index);
  }

  // BUG add management of the textures (upload them as necessary)
  void Scene::draw(bool alsoSynthesisPreProj) const
  {
    if (invalid)
      throw std::invalid_argument("Tried to draw an invalid Scene.");

    dbg::if_dlevel<dbg::Level::VERBOSE>([]()
                                        { std::cerr << "Drawing scene." << std::endl; });

    // upload the skybox if there is one.
    if (skybox)
    {
      if (skybox->shader->hasTexturing())
        skybox->shader->uploadTexture(skybox->model);
      skybox->draw(*this);
    }

    // Use underlying shader before uploading anything
    shader->useShader();
    // upload matices

    // if lighting is used in the scene, upload the Lights data to shader
    if (shader->hasLighting())
    {
      shader->uploadLighting(m_lights);
    }

    // If there is any model, initialize shaders' matrices with dummy
    if (not m_models.empty() or terrain != nullptr)
    {
      shader->uploadMatrix(Shader::Matrices::w2v, camera.matrix());
      shader->uploadMatrix(Shader::Matrices::proj, projectionMatrix);
      shader->uploadMatrix(Shader::Matrices::m2w, IdentityMatrix());
    }

    if (terrain)
      _helpers::uploadModelData(*this, terrain->model(), alsoSynthesisPreProj);

    // Draw every Model
    for (auto const &model : m_models)
    {
      // TODO call update on object ?
      _helpers::uploadModelData(*this, model, alsoSynthesisPreProj);
    }
    dbg::if_dlevel<dbg::Level::VERBOSE>([]()
                                        { std::cerr << "Scene drawn successfully" << std::endl; });
  }

} // closing of namespace