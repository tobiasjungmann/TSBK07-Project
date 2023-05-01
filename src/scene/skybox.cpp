#include "skybox.hpp"
#include "scene.hpp"
#include "debugging.h"

#include <iostream>

namespace scn {

Skybox::Skybox(std::unique_ptr<SkyboxShader> shaderp, Model* model) :
    m_shader{std::move(shaderp)}, model{model}
{}

// TODO add managment of the texture (upload)
void Skybox::draw(const Scene& scene) const
{
	dbg::if_dlevel<dbg::Level::VERBOSE>([](){std::cerr << "Trying to draw skybox." << std::endl;});
    shader->useShader();
	glDisable(GL_DEPTH_TEST); // required before drawing skybox
	glBindVertexArray(model->vao);
	// NO TRANSLATION for the world-to-view matrix of the skybox !!
	shader->uploadMatrix(Shader::Matrices::proj, scene.proj);
	shader->uploadMatrix(Shader::Matrices::preProj, zeroOutTranslation(scene.camera.matrix())); // do not translate with th ecamera !!

	glDrawElements(GL_TRIANGLES, model->numIndices, GL_UNSIGNED_INT, 0L);
	glEnable(GL_DEPTH_TEST); // enable it back after drawing skybox
	dbg::if_dlevel<dbg::Level::VERBOSE>([](){std::cerr << "Skybox drawing SUCCESS" << std::endl;});
}

Skybox::~Skybox() {
	DisposeModel(model);
}
}