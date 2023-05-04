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

namespace scn
{
    Scene::Scene()
        : camera{0, 0, 0},
          projectionMatrix{IdentityMatrix()},
          skybox{nullptr}
    {   invalid = true; }

    Scene::Scene(std::unique_ptr<SceneShader> shader, const Camera &camera, mat4 projectionMatrix, std::unique_ptr<Skybox> skybox)
        : camera{camera},
          projectionMatrix{projectionMatrix},
          m_shader{std::move(shader)},
          skybox{std::move(skybox)}
    {invalid = false;}

    Scene &Scene::operator=(Scene &&moved) noexcept
    {
        if (moved.invalid)
            throw std::invalid_argument("Tried to assign an invalid Scene.");
        m_shader = std::move(moved.m_shader);
        projectionMatrix = moved.projectionMatrix;
        camera = moved.camera;
        invalid = moved.invalid;
        skybox = std::move(moved.skybox);
        model_m2w = std::move(moved.model_m2w);

        dbg::if_dlevel<dbg::Level::VERBOSE>([]()
                                            { std::cerr << "move assigment" << std::endl; });

        return *this;
    }


    void Scene::popModel()
    {
        model_m2w.pop_back();
    }

    void Scene::pushModel(Modelv2 newMdl)
    {
        pushModel(newMdl, IdentityMatrix());
    }

    void Scene::pushModel(Modelv2 newMdl, mat4 m2wMtx)
    {
        model_m2w.emplace_back(newMdl, m2wMtx);
    }

    void Scene::updateModelM2W(long modelIndex, mat4 update)
    {
        if (modelIndex < 0)
            modelIndex += model_m2w.size();
        model_m2w[modelIndex].second = update;
    }

    void Scene::addLightSource(const Light &light)
    {
        if (not shader->hasLighting() && ! ENV_NOTHROW)
            throw std::logic_error("Attempt to add light sources without prior initialization of the lighting engine");
        lights.push_back(light);
    }

    void Scene::removeLightSource(long index)
    {
        if (not shader->hasLighting() && ! ENV_NOTHROW)
            throw std::logic_error("Attempt to add light sources without prior initialization of the lighting engine");

        if (index < 0)
            index += lights.size();

        lights.erase(std::begin(lights) + index);
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
            shader->uploadLighting(lights);
        }

        // no need for upload if no models to render
        if (!model_m2w.empty())
        {
            shader->uploadMatrix(Shader::Matrices::w2v, camera.matrix());
            shader->uploadMatrix(Shader::Matrices::proj, projectionMatrix);
        }

        shader->uploadMatrix(Shader::Matrices::m2w, IdentityMatrix());


        // Draw every Model
        for (auto const& model_m2w_pair : model_m2w)
        {
            auto const& model = model_m2w_pair.first;
            auto m2w = model_m2w_pair.second;
            glBindVertexArray(model.get()->vao);
            if (alsoSynthesisPreProj) {
                shader->uploadMatrix(Shader::Matrices::preProj, getPreProj(m2w));
            }

            // if (m2w != IdentityMatrix()) // BUG improve that performance wise (store pointer maybe ?)
                shader->uploadMatrix(Shader::Matrices::m2w, m2w);
            // upload material of 
            if (shader->hasLighting()) {
                shader->uploadModelLightProps(model.getLightProps());
            }

            if (shader->hasTexturing())
                shader->uploadTexture(model.get());
            glDrawElements(GL_TRIANGLES, model.get()->numIndices, GL_UNSIGNED_INT, 0L);
        }
        dbg::if_dlevel<dbg::Level::VERBOSE>([]()
                                         { std::cerr << "Scene drawn successfully" << std::endl; });
    }

} // closing of namespace

// when to use a reference wrapper and when to use a shared ptr ?