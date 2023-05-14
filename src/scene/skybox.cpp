#include "skybox.hpp"
#include "scene.hpp"
#include "debugging.h"
#include "modelv2.hpp"

#include <iostream>

namespace scn
{

  Skybox::Skybox(std::unique_ptr<SkyboxShader> shaderp, Modelv2 model) : m_shader{std::move(shaderp)}, model{model}
  {
  }

  Skybox::~Skybox() {
    m_shader.reset();
  }

  // TODO add managment of the texture (upload)
  void Skybox::draw(const Scene &scene) const
  {
    dbg::if_dlevel<dbg::Level::VERBOSE>([]()
                                        { std::cerr << "Trying to draw skybox." << std::endl; });
    shader->useShader();
    glDisable(GL_DEPTH_TEST); // required before drawing skybox
    glBindVertexArray(model.get()->vao);
    // NO TRANSLATION for the world-to-view matrix of the skybox !!
    shader->uploadMatrix(Shader::Matrices::proj, scene.proj);
    shader->uploadMatrix(Shader::Matrices::preProj, zeroOutTranslation(scene.camera.matrix())); // do not translate with th ecamera !!

    glDrawElements(GL_TRIANGLES, model.get()->numIndices, GL_UNSIGNED_INT, 0L);
    glEnable(GL_DEPTH_TEST); // enable it back after drawing skybox
    dbg::if_dlevel<dbg::Level::VERBOSE>([]()
                                        { std::cerr << "Skybox drawing SUCCESS" << std::endl; });
  }
}