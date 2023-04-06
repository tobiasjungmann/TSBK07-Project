#pragma once

#include "LittleOBJLoader.h"
#include <vector>
#include <utility>
#include <memory>
#include <initializer_list>
#include <cstddef>
#include "camera.hpp"
#include "skybox.hpp"
#include "light.hpp"

namespace scn
{
class Scene
{
public:
  Scene ();
  /// @brief Construct a scene without prior information of the projection matrix
  /// @param shader A pointer to shader instance, whose ownership will be acquired by the scene.
  /// @param camera The camera used for this Scenes
  /// @param projectionMatrix The projection matrix shared between all models of the scene, sent to the shaders" "projName" uniform
  /// @param skybox An optional skybox's pointer. Ownership onto the underlying skybox must be given to the Scene.
  Scene (std::unique_ptr<SceneShader> shader, 
   const Camera &camera, 
   mat4 projectionMatrix, 
   std::unique_ptr<Skybox> skybox = nullptr);
  Scene& operator= (const Scene &cpy) = delete;
  Scene& operator= (Scene &&cpy) noexcept;
  ~Scene() = default;
  
  // Model & Transformation Management
  /**
   * @brief Remove the last pushed model
   * 
   */
  void popModel ();
  /**
   * @brief Add a NEWMDL to the scene, with an optional M2W matrix. 
   * 
   * @param newMdl 
   * @param m2wMtx 
   */
  void pushModel (std::shared_ptr<Model> newMdl, std::shared_ptr<mat4> m2wMtx = nullptr); // TODO should returns index of insertion in vector
  
  
  /**
   * @brief Update the model-to-world matrix with UPDATE of the model at MODELINDEX
   * 
   * @param modelIndex The index of the model to be updated. 
   * @param update The model-to-world matrix that should replace the current one
   */
  void updateModelM2W (long modelIndex, std::shared_ptr<mat4> update);
  
  // Light management
  void addLightSource(const Light &light);
  void removeLightSource(long index);
  
  void draw(bool alsoSynthesisPreProj = false) const;


private:  
  inline std::shared_ptr<mat4> getM2W (std::size_t index) const {
    return model_m2w[index].second;
  }

  inline mat4 getPreProj(std::size_t index) const {
    auto m2w = getM2W(index);
    return camera.matrix () * (m2w == nullptr ? IdentityMatrix () : *m2w);
  }
  
  inline mat4 getPreProj(std::shared_ptr<mat4> m2w) const {
    return camera.matrix () * (m2w == nullptr ? IdentityMatrix () : *m2w);
  }


public:
  Camera camera;
  union {
    mat4 proj;
    mat4 projectionMatrix;
  };

  const std::unique_ptr<SceneShader>& shader {m_shader};
  std::unique_ptr<Skybox> skybox;

private:
  // TODO compare performance with a vector<pair<std::shared_ptr<Model>, vector<mat4>>> (comparison of cache proximity)
  std::vector<std::pair<std::shared_ptr<Model>, std::shared_ptr<mat4>>> model_m2w;
  
  std::vector<vec4> lightSourcesIntensities;
  std::vector<vec3> lightSourcesDirections;
  std::vector<int> lightSourcesDirectionalities; /// int instead of bools, cuz we need the underlying array with ints
  
  std::unique_ptr<SceneShader> m_shader;
  bool invalid;
};
} // end namespace scene