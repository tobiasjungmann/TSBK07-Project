#pragma once

#include "LittleOBJLoader.h"
#include <vector>
#include <utility>
#include <memory>
#include <cstddef>
#include "camera.hpp"
#include "skybox.hpp"
#include "terrain.hpp"
#include "gameobj/light.hpp"

#include "modelv2.hpp"

namespace _helpers
{
  void uploadModelData(scn::Scene const &scene, Modelv2 model, bool alsoSynthesisPreProj);
}

namespace scn
{
  class Scene
  {
  private:
    friend void _helpers::uploadModelData(Scene const &scene, Modelv2 model, bool alsoSynthesisPreProj);

  public:
    Scene();
    /// @brief Construct a scene without prior information of the projection matrix
    /// @param shader A pointer to shader instance, whose ownership will be acquired by the scene.
    /// @param camera The camera used for this Scenes
    /// @param projectionMatrix The projection matrix shared between all models of the scene, sent to the shaders" "projName" uniform
    /// @param skybox An optional skybox's pointer. Ownership onto the underlying skybox must be given to the Scene.
    Scene(std::unique_ptr<SceneShader> shader,
          const Camera &camera,
          mat4 projectionMatrix,
          std::unique_ptr<Terrain> terrain = nullptr,
          std::unique_ptr<Skybox> skybox = nullptr);
    Scene &operator=(const Scene &cpy) = delete;
    Scene &operator=(Scene &&cpy) noexcept;
    ~Scene() = default;

    // Model & Transformation Management
    /**
     * @brief Remove the last pushed model
     *
     */
    void popModel();
    /**
     * @brief Add a NEWMDL to the scene, with an optional M2W matrix.
     *
     * @param newMdl
     * @param m2wMtx
     */
    void pushModel(Modelv2 newMdl);              // TODO should returns index of insertion in vector
    void pushModel(Modelv2 newMdl, mat4 m2wMtx); // TODO should returns index of insertion in vector

    /**
     * @brief Update the model-to-world matrix with UPDATE of the model at MODELINDEX
     *
     * @param modelIndex The index of the model to be updated.
     * @param update The model-to-world matrix that should replace the current one
     */
    void updateModelM2W(long modelIndex, mat4 update);

    // Light management
    void addLightSource(const Light &light);
    void removeLightSource(long index);

    void draw(bool alsoSynthesisPreProj = false) const;

  public:
    Camera camera;
    union
    {
      mat4 proj;
      mat4 projectionMatrix;
    };

    std::unique_ptr<SceneShader> const &shader{m_shader};
    std::unique_ptr<Terrain> terrain = nullptr;
    std::unique_ptr<Skybox> skybox = nullptr;

  private:
    std::vector<Modelv2> m_models;
    std::vector<Light> m_lights;

    std::unique_ptr<SceneShader> m_shader = nullptr;
    bool invalid;
  };
} // end namespace scene