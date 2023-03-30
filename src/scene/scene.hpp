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
        Camera camera;
        union {
            std::shared_ptr<mat4> proj;
            std::shared_ptr<mat4> projectionMatrix;
        };
    private:
        // TODO compare performance with a vector<pair<Model*, vector<mat4>>> (comparison of cache proximity)
        // TODO use of pointers would reduce the duplication of data
        std::vector<std::pair<Model *, std::shared_ptr<mat4>>> modelsTransfo;

        std::vector<vec4> lightSourcesIntensities;
        std::vector<vec3> lightSourcesDirections;
        std::vector<int> lightSourcesDirectionalities; /// int instead of bools, cuz we need the underlying array with ints

        std::unique_ptr<SceneShader> m_shader;
        bool invalid;

        void freeModelsVector();
        inline std::shared_ptr<mat4> getM2W(std::size_t index) const;
        inline mat4 getPreProj(std::size_t index) const;
        inline mat4 getPreProj(std::shared_ptr<mat4> m2w) const;
    public:
        const std::unique_ptr<SceneShader>& shader {m_shader};
        Skybox *skybox;

        Scene();
        /// @brief Construct a scene without prior information of the projection matrix
        /// @param shader A pointer to shader instance, whose ownership will be acquired by the scene.
        /// @param camera The camera used for this Scenes
        Scene(std::unique_ptr<SceneShader>shader, const Camera &camera, std::shared_ptr<mat4> projectionMatrix);
        /// @brief Construct a scene without prior information of the projection matrix
        /// @param shader A pointer to shader instance, whose ownership will be acquired by the scene.
        /// @param camera The camera used for this Scenes
        /// @param projectionMatrix The projection matrix shared between all models of the scene, sent to the shaders" "projName" uniform
        /// @param skybox An optional skybox's pointer. Ownership onto the underlying skybox must be given to the Scene.
        Scene(std::unique_ptr<SceneShader>shader, const Camera &camera, std::shared_ptr<mat4> projectionMatrix, Skybox *&&skybox);
        Scene &operator=(const Scene &cpy) = delete;
        Scene &operator=(Scene &&cpy) noexcept;

        ~Scene();

        // Model & Transformation Management
        void popModel();
        void pushModel(Model *newMdl, std::shared_ptr<mat4> m2wMtx = nullptr); // TODO should returns index of insertion in vector
        

        void updateTransfo(long modelIndex, std::shared_ptr<mat4> update);
        inline void setM2W(std::size_t index, std::shared_ptr<mat4> value);


        // Light management
        void addLightSource(const Light &light);
        void removeLightSource(long index);

        void draw(bool alsoSynthesisPreProj = false) const;

    };

} // closing bracket of namespace scene

#include "scene.tcc"