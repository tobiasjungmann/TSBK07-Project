#include "shaders.hpp"

#include "GL_utilities.h"
#include <stdexcept>
#include <algorithm>
#include "LittleOBJLoader.h"
#include "types_properties.hpp"

using std::size_t;

namespace scn
{
    Shader::Shader(const char *vertexShader, const char *fragmentShader, std::array<const char*, Shader::max_mtx> const& matriceVarsNames)
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
        std::array<const char *, max_mtx> const& matriceVarsNames)
        : Shader(vertexShader, fragmentShader, matriceVarsNames)
    {
        initTexturing(texUnitVarName, textureVarName);
    }

    void Shader::resetShaderDataLocation(Shader::Matrices index, const char* shaderVarName) {
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
        if (matricesLoc[i] < 0)
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
        glUseProgram(hndl); // FIXME handle error
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

    void SceneShader::initLighting(const char *lightSourcesDirPosArrName,
                                   const char *lightSourcesColorArrName,
                                   const char *lightDirectionalitiesArrName,
                                   const char *specularExponentVarName)
    {
        useShader();
        lightDirsMtxLoc = glGetUniformLocation(hndl, lightSourcesDirPosArrName);
        lightIntensitiesMtxLoc = glGetUniformLocation(hndl, lightSourcesColorArrName);
        lightDirecnalMtxLoc = glGetUniformLocation(hndl, lightDirectionalitiesArrName);
        specExponentLoc = glGetUniformLocation(hndl, specularExponentVarName);
        lighting = true;
    }

    void SceneShader::uploadLighting(size_t nbLights,
                                     const GLfloat *sourcesDirs,
                                     const GLfloat *sourcesIntensities,
                                     const GLint *sourcesDirnalties) const
    {
        if (not lighting)
            throw std::logic_error("Lighting was not properly initialized before attempting data upload");
        useShader();
        glUniform3fv(lightDirsMtxLoc, nbLights, sourcesDirs);
        glUniform3fv(lightIntensitiesMtxLoc, nbLights, sourcesIntensities);
        glUniform1iv(lightDirecnalMtxLoc, nbLights, sourcesDirnalties);
    }

    void SceneShader::uploadSpecularExponent(GLfloat exponent) const
    {
        if (not lighting)
            throw std::logic_error("Lighting was not properly initialized before attempting specular exponent upload");
        useShader();
        glUniform1f(specExponentLoc, exponent);
    }

    SkyboxShader::SkyboxShader(const char *vertexShader, const char *fragmentShader, const char *projMtxVarName, const char *preProjMtxVarName)
        : Shader(vertexShader, fragmentShader, {projMtxVarName, preProjMtxVarName})
    {}

    SkyboxShader::SkyboxShader(
        const char *vertexShader,
        const char *fragmentShader,
        const char *projMtxVarName,
        const char *preProjMtxVarName,
        const char *texUnitVarName,
        const char *textureVarName)
        : Shader(vertexShader, fragmentShader, texUnitVarName, textureVarName, {projMtxVarName, preProjMtxVarName})
    {}
}