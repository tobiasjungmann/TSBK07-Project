#pragma once

#include "LittleOBJLoader.h"

#include "shaders.hpp"
#include "../modelv2.hpp"
#include <memory>

namespace scn {
class Scene;

class Skybox {
    private:
    std::unique_ptr<SkyboxShader> m_shader;
    
    public:
    const std::unique_ptr<SkyboxShader>& shader{m_shader};
    Modelv2 model;


    // TODO add texture management to skybox
    Skybox(std::unique_ptr<SkyboxShader> shaderp, Modelv2 modelp);
    constexpr Skybox(Skybox const&) = delete;
    constexpr Skybox& operator=(Skybox const&) = delete;
    ~Skybox();
    void draw(const Scene& scene) const;
};
}