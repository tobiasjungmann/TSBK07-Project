#include "shaders.hpp"

#include "GL_utilities.h"
#include <stdexcept>
#include <algorithm>
#include "LittleOBJLoader.h"
#include "types_properties.hpp"
#include "debugging.h"
#include "light.hpp"

using std::size_t;

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

  void Shader::uploadTexture(const Model *model) const
  {
    uploadTexture(model, model->material->texRefID, model->material->texUnit);
  }

  void Shader::uploadTexture(const Model *model, GLuint textureReferenceID, GLuint textureUnit) const
  {
    if (not texturized)
      throw std::logic_error("Texture was not properly initialized before attempting texture data upload");

    else if (not model->material)
      throw std::logic_error("Given model has no material at the ready for a texture.");

    model->material->texRefID = textureReferenceID;
    model->material->texUnit = textureUnit;
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

    for (std::size_t i = 0; i < lights.size(); i++)
    {
      std::string currentLight = lightStructArrayName + '[' + std::to_string(i) + ']';
      GLint lightDirLoc = glGetUniformLocation(hndl, (currentLight + ".dirPos").c_str());
      GLint lightColorLoc = glGetUniformLocation(hndl, (currentLight + ".color").c_str());
      GLint lightDirectionalLoc = glGetUniformLocation(hndl, (currentLight + ".directional").c_str());

      if ((lightDirLoc == -1 or lightColorLoc == -1 or lightDirectionalLoc == -1) && !ENV_NOTHROW)
        throw std::logic_error("Light " + std::to_string(i) + " could not be properly located in shader");

      glUniform3f(lightDirLoc, lights[i].dirPos.x, lights[i].dirPos.y, lights[i].dirPos.z);
      glUniform3f(lightColorLoc, lights[i].intensity.x, lights[i].intensity.y, lights[i].intensity.z);
      glUniform1i(lightDirectionalLoc, lights[i].directional);
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
      GLint ambiantCoeffLoc = glGetUniformLocation(hndl, (mtlPropsStructName + ".ambiantCoeff").c_str());
      GLint alphaLoc = glGetUniformLocation(hndl, (mtlPropsStructName + ".specularExponent").c_str());

      if (diffusenessLoc != -1)
        glUniform1f(diffusenessLoc, props->diffuseness);
      if (specularityLoc != -1)
        glUniform1f(specularityLoc, props->specularity);
      if (alphaLoc != -1)
        glUniform1f(alphaLoc, props->specularExponent);
      if (ambiantCoeffLoc != -1)
        glUniform1f(ambiantCoeffLoc, props->ambiantCoeff);
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