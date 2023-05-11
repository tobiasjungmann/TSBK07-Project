#pragma once

#include "VectorUtils4.h"
#include "types_properties.hpp"
#include <cstddef>
#include <array>
#include <vector>
#include <string>
#include "modelv2.hpp"
#include "gameobj/light.hpp"


struct Model;


namespace scn
{
    class Shader
    {
    public:
        enum class Matrices : std::size_t
        {
            proj = 0,
            preProj = 1, // reserved spot for preProj
            m2w = 2,
            w2v = 3,
            extra1 = 5,
            extra2 = 6,
        };
        const GLuint hndl;
        constexpr static GLint max_mtx = 6;
        std::size_t nb_mtx; 

    private:
        // Never more than 6 matrices would be practically used to render a vertex in the shaders.
        // We waste at most a 4*sizeof(GLint) of memory, but it should be preferred to the code bloating
        // we would get with a template.
        GLint matricesLoc[max_mtx];

        GLint texUnitLoc = -1;
        GLint textureVarLoc = -1;

    protected:
        bool texturized = false;

        Shader(const char *vertexShaderFilename, const char *fragmentShaderFilename, std::array<const char *, max_mtx> const& matriceVarsNames);
        Shader(
            const char *vertexShaderFilename,
            const char *fragmentShaderFilename,
            const char *texUnitVarName,
            const char *textureVarName,
            std::array<const char *, max_mtx> const& matriceVarsNames);
    public:
        /**
         * @brief Update the glLocation of the matrix stored at INDEX with 
         * the location of SHADERVARNAME within the shader. 
         * 
         * @param index 
         * @param shaderVarName 
         */
        void resetShaderDataLocation(Shader::Matrices index, const char* shaderVarName);
        void initTexturing(const char *texUnitVarName, const char *textureVarName);
        inline bool hasTexturing() const noexcept { return texturized; }

        /**
         * @brief Upload data to the first (index 0) matrix if it exists
         * as per shortcut index 0 is the projection matrix
         * @param matrixData
         */
        void uploadMatrix(Matrices index, const mat4 &matrixData) const;
        template <class InputIt>
        void uploadMatrices(std::size_t first, InputIt firstData, InputIt lastData) const;
        void uploadTexture(const Model *model) const;
        void uploadTexture(const Model *model, GLuint textureReferenceID, GLuint textureUnit) const;
        void useShader() const;
    };

    /**
     * @brief The Shader object used by every Skybox object
     *
     */
    class SkyboxShader : public Shader
    {
    private:
    public:
        SkyboxShader(
            const char *vertexShader,
            const char *fragmentShader,
            const char *projMtxVarName,
            const char *preProjMtxVarName);
        SkyboxShader(
            const char *vertexShader,
            const char *fragmentShader,
            const char *projMtxVarName,
            const char *preProjMtxVarName,
            const char *texUnitVarName,
            const char *textureVarName);
    };

    /**
     * @brief The Shader object used by every Scene object
     *
     */
    class SceneShader : public Shader
    {

    public:
        SceneShader(
            const char *vertexShader,
            const char *fragmentShader,
            const char *projMtxVarName,
            const char *worldToViewMtxName,
            const char *modelToWorldMtxName);

        SceneShader(
            const char *vertexShader,
            const char *fragmentShader,
            const char *projMtxVarName,
            const char *preProjMtxVarName);


        void initLighting(std::string const& lightStructArrayName);
        void initMaterialProps(std::string const& materialPropsStructName);

        inline bool hasLighting() const noexcept { return not lightStructArrayName.empty(); }

        void uploadLighting(std::vector<Light> const& lights) const;
        void uploadModelLightProps(const Modelv2::MaterialLight* props) const;

    private:
        std::string lightStructArrayName;
        std::string mtlPropsStructName;
    };
}
