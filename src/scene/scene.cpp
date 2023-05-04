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

    void Scene::pushModel(const Model *newMdl)
    {
        pushModel(newMdl, IdentityMatrix());
    }

    void Scene::pushModel(const Model *newMdl, mat4 m2wMtx)
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
        if (not shader->hasLighting())
            throw std::logic_error("Attempt to add light sources without prior initialization of the lighting engine");
        lightSourcesIntensities.push_back(light.intensity);
        lightSourcesDirections.push_back(light.direction);
        lightSourcesDirectionalities.push_back(light.directional);
    }

    void Scene::removeLightSource(long index)
    { // TODO maybe consider std::set::extract
        if (not shader->hasLighting())
            throw std::logic_error("Attempt to add light sources without prior initialization of the lighting engine");

        if (index < 0)
            index += lightSourcesIntensities.size();

        lightSourcesIntensities.erase(lightSourcesIntensities.begin() + index);
        lightSourcesDirectionalities.erase(lightSourcesDirectionalities.begin() + index);
        lightSourcesDirections.erase(lightSourcesDirections.begin() + index);
    }

    // TODO add management of the textures (upload them as necessary)
    void Scene::draw(bool alsoSynthesisPreProj) const
    {
        std::cout << camera<<"\n";
        if (invalid)
            throw std::invalid_argument("Tried to draw an invalid Scene.");

        dbg::if_dlevel<dbg::Level::VERBOSE>([]()
                                         { std::cerr << "Drawing scene." << std::endl; });



        if (skybox)
        {
        // TODO maybe add a conditional on the upload of the skybox texture to limit the number of uploads
            if (skybox->shader->hasTexturing())
                skybox->shader->uploadTexture(skybox->model);
            skybox->draw(*this);
        }


        shader->useShader();
        // upload matices

        if (shader->hasLighting())
        {
            shader->uploadLighting(lightSourcesIntensities.size(),
                                   &lightSourcesDirections.data()[0].x,
                                   &lightSourcesIntensities.data()[0].x,
                                   lightSourcesDirectionalities.data());
        }

        // no need for upload if no models to render
        if (!model_m2w.empty())
        {
            shader->uploadMatrix(Shader::Matrices::w2v, camera.matrix());
            shader->uploadMatrix(Shader::Matrices::proj, projectionMatrix);
        }

        shader->uploadMatrix(Shader::Matrices::m2w, IdentityMatrix());


        for (auto &model_m2w_pair : model_m2w)
        {
            auto model = model_m2w_pair.first;
            auto m2w = model_m2w_pair.second;
            glBindVertexArray(model->vao);
            // TODO apply transfo only if one was provided
            if (alsoSynthesisPreProj) {
                shader->uploadMatrix(Shader::Matrices::preProj, getPreProj(m2w));
            }
            if (m2w != IdentityMatrix()) // TODO improve that performance wise (store pointer maybe ?)
                shader->uploadMatrix(Shader::Matrices::m2w, m2w);
            if (shader->hasLighting() && model->material)
                shader->uploadSpecularExponent(model->material->alpha);

            if (shader->hasTexturing())
                shader->uploadTexture(model);
            glDrawElements(GL_TRIANGLES, model->numIndices, GL_UNSIGNED_INT, 0L);
        }
        dbg::if_dlevel<dbg::Level::VERBOSE>([]()
                                         { std::cerr << "Scene drawn successfully" << std::endl; });
    }

} // closing of namespace

// when to use a reference wrapper and when to use a shared ptr ?