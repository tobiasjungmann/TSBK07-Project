#include "shaders.hpp"

#include "GL_utilities.h"
#include <stdexcept>
#include <algorithm>
#include <iostream>
#include "LittleOBJLoader.h"
#include "types_properties.hpp"
#include "debugging.h"
#include "gameobj/light.hpp"
#include "../modelv2.hpp"

using std::size_t;

namespace
{
  // SPECIFY gpuSlot to override the value
  void pushTexture(Modelv2 const &mdl, GLuint program, GLint texUnitLoc = -1, GLint gpuSlot = -1)
  {
    if (not mdl.hasTexture())
      throw std::runtime_error("Tried to push the texture for a model that has none.");

    if (texUnitLoc < 0)
    {
      std::cerr << "Couldn't not find the texture location in shader. " << std::endl;
      return;
    }

    glUseProgram(program);

    gpuSlot = (gpuSlot < 0) ? mdl.texture().texGPUSlot : gpuSlot;
    if (mdl.get()->texCoordArray)
    {
      glActiveTexture(GL_TEXTURE0 + gpuSlot);
      glBindTexture(GL_TEXTURE_2D, mdl.texture().texID);
      glUniform1i(texUnitLoc, gpuSlot);
    }
    else
    {
      throw std::runtime_error("pushTexture: Raw model didn't have texCoordArray");
    }
  }
}

namespace scn
{
  Shader::Shader(const char *vertexShader, const char *fragmentShader, std::array<const char *, Shader::max_mtx> const &matriceVarsNames)
      : hndl{loadShaders(vertexShader, fragmentShader)}, nb_mtx{0}
  {
    useShader();
    for (auto i = 0; i < Shader::max_mtx; ++i)
      matricesLoc[i] = (matriceVarsNames[i]) ? nb_mtx++, glGetUniformLocation(hndl, matriceVarsNames[i]) : -1;
  }

  Shader::Shader(
      const char *vertexShader,
      const char *fragmentShader,
      const char *texUnitVarName,
      const char *textureVarName,
      std::array<const char *, max_mtx> const &matriceVarsNames)
      : Shader(vertexShader, fragmentShader, matriceVarsNames)
  {
    initTexturing(texUnitVarName, textureVarName);
  }

  void Shader::resetShaderDataLocation(Shader::Matrices index, const char *shaderVarName)
  {
    useShader();
    auto i{to_underlying(index)};
    if (matricesLoc[i] < 0) // TODO careful here in case next line fails
      nb_mtx++;
    matricesLoc[i] = glGetUniformLocation(hndl, shaderVarName); // FIXME handle errors for all such calls
                                                                /*
                                                                    if failure -> nb_mtx back to a valid state (decrement if required)
                                                                */
  }

  void Shader::uploadMatrix(Shader::Matrices index, const mat4 &matrixData) const
  {
    useShader();
    auto i{to_underlying(index)};
    if (matricesLoc[i] < 0 && !ENV_NOTHROW)
      throw std::logic_error("Attempted to upload a matrix whose shader's location was unitialized.");
    glUniformMatrix4fv(matricesLoc[i], 1, GL_TRUE, matrixData.m);
  }

  template <class InputIt>
  void Shader::uploadMatrices(size_t first, InputIt firstData, InputIt lastData) const // upload a contigous array of matrices
  {
    if (lastData - firstData + first > Shader::max_mtx or lastData - firstData > nb_mtx)
      throw std::out_of_range("Attempted to upload undefined matrices");
    useShader();

    for (auto data_it = firstData, i = first; data_it != lastData; data_it++, i++)
    {
      while (i < Shader::max_mtx and matricesLoc[i] < 0)
        i++;
      if (i == Shader::max_mtx)
        throw std::logic_error("Not enough initialized shader location in the given range to upload the provided data.");
      glUniformMatrix4fv(matricesLoc[i], 1, GL_TRUE, data_it->m);
    }
  }

  void Shader::initTexturing(const char *texUnitVarName, const char *textureVarName)
  {
    if (not texUnitVarName or not textureVarName)
      throw std::invalid_argument("Cannot initialize texturing with a NULL texture unit variable's name");

    useShader();
    texUnitLoc = glGetUniformLocation(hndl, texUnitVarName);
    textureVarLoc = glGetUniformLocation(hndl, textureVarName);
    // TODO check error codes of glGetUniformLocation.
    texturized = true;
  }

  void Shader::uploadTexture(Model *raw) const
  {
    if (not raw->material)
      throw std::runtime_error("Tried to upload texture for a raw model without.");

    // gpuSlot = (gpuSlot < 0) ? mdl->material->texUnit : gpuSlot;
    auto model {Modelv2(raw, Modelv2::MaterialTexture{raw->material->texRefID})};
    uploadTexture(model, model.texture().texID, model.texture().texGPUSlot);
  }

  void Shader::uploadTexture(Modelv2 const &model) const
  {
    uploadTexture(model, model.texture().texID, model.texture().texGPUSlot);
  }

  void Shader::uploadTexture(Modelv2 const &model, GLuint textureReferenceID, GLuint textureUnit) const
  {
    if (not texturized)
      throw std::logic_error("Texture was not properly initialized before attempting texture data upload");

    else if (not model.hasTexture())
      throw std::logic_error("Raw model has no material at the ready for a texture.");

    pushTexture(model, hndl, texUnitLoc);
  }

  void Shader::useShader() const
  {
    glUseProgram(hndl); // TODO handle error of this call
  }

  SceneShader::SceneShader(const char *vertexShader,
                           const char *fragmentShader,
                           const char *projMtxVarName,
                           const char *worldToViewMtxName,
                           const char *modelToWorldMtxName)
      : Shader(vertexShader, fragmentShader, {projMtxVarName, nullptr, modelToWorldMtxName, worldToViewMtxName}) {}

  SceneShader::SceneShader(const char *vertexShader,
                           const char *fragmentShader,
                           const char *projMtxVarName,
                           const char *preProjMtxVarName) : Shader(vertexShader, fragmentShader, {projMtxVarName, preProjMtxVarName}) {}

  void SceneShader::initLighting(std::string const &lightStructArrayName)
  {
    useShader();
    if (!ENV_NOTHROW && glGetUniformLocation(hndl, (lightStructArrayName + "[0].color").c_str()) == -1)
      throw std::logic_error(
          "Failure when initializing lighting. Variable " + std::string(lightStructArrayName) + " not found in shader.");
    this->lightStructArrayName = lightStructArrayName;
  }

  void SceneShader::initMaterialProps(std::string const &materialPropsStructName)
  {
    useShader();
    if (!ENV_NOTHROW && glGetUniformLocation(hndl, (materialPropsStructName + ".specularity").c_str()) == -1)
      throw std::logic_error(
          "Failure when initializing material props. Variable " + std::string(materialPropsStructName) + " not found in shader.");
    this->mtlPropsStructName = materialPropsStructName;
  }

  void SceneShader::uploadLighting(std::vector<Light> const &lights) const
  {
    if (!ENV_NOTHROW && not hasLighting())
      throw std::logic_error("Lighting was not properly initialized before attempting data upload");
    useShader();

    auto report = [](std::size_t i)
    {
      // std::cerr << "Light " + std::to_string(i) + " could not be properly located in shader";
    };

    for (std::size_t i = 0; i < lights.size(); i++)
    {
      auto &&lightStr = lightStructArrayName + '[' + std::to_string(i) + ']';
      Light const &curr{lights[i]};

      GLint colorLoc = glGetUniformLocation(hndl, (lightStr + ".color").c_str());
      GLint directionalLoc = glGetUniformLocation(hndl, (lightStr + ".directional").c_str());
      GLint attenuateLoc = glGetUniformLocation(hndl, (lightStr + ".attenuate").c_str());
      GLint spotlightLoc = glGetUniformLocation(hndl, (lightStr + ".spotlight").c_str());
      GLint inViewLoc = glGetUniformLocation(hndl, (lightStr + ".inView").c_str());
      GLint ambientKLoc = glGetUniformLocation(hndl, (lightStr + ".ambientK").c_str());
      GLint diffuseKLoc = glGetUniformLocation(hndl, (lightStr + ".diffuseK").c_str());
      GLint specularKLoc = glGetUniformLocation(hndl, (lightStr + ".specularK").c_str());
      if ((colorLoc == -1 or attenuateLoc == -1 or directionalLoc == -1 or spotlightLoc == -1 or inViewLoc == -1 or ambientKLoc == -1 or diffuseKLoc == -1 or specularKLoc == -1) && !ENV_NOTHROW)
        report(i);

      glUniform3f(colorLoc, curr.color.x, curr.color.y, curr.color.z);
      glUniform1i(directionalLoc, curr.directional);
      glUniform1i(attenuateLoc, curr.attenuate);
      glUniform1i(spotlightLoc, curr.spotlight);
      glUniform1i(inViewLoc, curr.position.inView);
      glUniform1f(ambientKLoc, curr.ambientK);
      glUniform1f(diffuseKLoc, curr.diffuseK);
      glUniform1f(specularKLoc, curr.specularK);

      if (curr.attenuate)
      {
        GLint constLoc, linearLoc, squareLoc;
        constLoc = glGetUniformLocation(hndl, (lightStr + ".attenuConst").c_str());
        linearLoc = glGetUniformLocation(hndl, (lightStr + ".attenuLinear").c_str());
        squareLoc = glGetUniformLocation(hndl, (lightStr + ".attenuSquare").c_str());
        if (constLoc == -1 or linearLoc == -1 or squareLoc == -1)
          report(i);
        glUniform1f(constLoc, curr.attenuConst);
        glUniform1f(linearLoc, curr.attenuLinear);
        glUniform1f(squareLoc, curr.attenuSquare);
      }

      if (curr.spotlight)
      {
        GLint cutoffLoc = glGetUniformLocation(hndl, (lightStr + ".cutOff").c_str());
        GLint outerLoc = glGetUniformLocation(hndl, (lightStr + ".outerEdgeCutOff").c_str());
        if (cutoffLoc == -1 or outerLoc == -1)
          report(i);
        glUniform1f(cutoffLoc, curr.cutOff);
        glUniform1f(outerLoc, curr.outerEdgeCutOff);
      }

      if (curr.directional or curr.spotlight)
      {
        if (GLint dirLoc = glGetUniformLocation(hndl, (lightStr + ".direction").c_str()); dirLoc != -1)
        {
          glUniform3f(dirLoc, curr.direction().x, curr.direction().y, curr.direction().z);
        }
        else
          report(i);
      }
      if (not curr.directional or curr.spotlight)
      {
        if (GLint posLoc = glGetUniformLocation(hndl, (lightStr + ".position").c_str()); posLoc != -1)
          glUniform3f(posLoc, curr.position().x, curr.position().y, curr.position().z);
        else
          report(i);
      }
    }
  }

  void SceneShader::uploadModelLightProps(const Modelv2::MaterialLight *props) const
  {
    if (not hasLighting() && !ENV_NOTHROW)
      throw std::logic_error("Lighting was not properly initialized before attempting model light properties upload");
    if (props)
    {
      if (!ENV_NOTHROW && mtlPropsStructName.empty())
      {
        throw std::logic_error("Material properties were not initialized before attempting model light properties upload");
      }

      useShader();
      GLint diffusenessLoc = glGetUniformLocation(hndl, (mtlPropsStructName + ".diffuseness").c_str());
      GLint specularityLoc = glGetUniformLocation(hndl, (mtlPropsStructName + ".specularity").c_str());
      GLint ambientCoeffLoc = glGetUniformLocation(hndl, (mtlPropsStructName + ".ambientCoeff").c_str());
      GLint alphaLoc = glGetUniformLocation(hndl, (mtlPropsStructName + ".specularExponent").c_str());

      if (diffusenessLoc != -1)
        glUniform1f(diffusenessLoc, props->diffuseness);
      if (specularityLoc != -1)
        glUniform1f(specularityLoc, props->specularity);
      if (alphaLoc != -1)
        glUniform1f(alphaLoc, props->specularExponent);
      if (ambientCoeffLoc != -1)
        glUniform1f(ambientCoeffLoc, props->ambientCoeff);
    }
  }

  SkyboxShader::SkyboxShader(const char *vertexShader, const char *fragmentShader, const char *projMtxVarName, const char *preProjMtxVarName)
      : Shader(vertexShader, fragmentShader, {projMtxVarName, preProjMtxVarName})
  {
  }

  SkyboxShader::SkyboxShader(
      const char *vertexShader,
      const char *fragmentShader,
      const char *projMtxVarName,
      const char *preProjMtxVarName,
      const char *texUnitVarName,
      const char *textureVarName)
      : Shader(vertexShader, fragmentShader, texUnitVarName, textureVarName, {projMtxVarName, preProjMtxVarName})
  {
  }
}