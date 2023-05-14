#include "resource_manager.hpp"
#include <stdexcept>
#include <string>
#include "debugging.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define MODELS_PFX_STR "../rc/models/"
#define MODELS_PFX_LEN 13

using namespace rc;

bool ResourceManager::initialized{false};
GLuint ResourceManager::program;
std::string ResourceManager::vertexVarName;
std::string ResourceManager::normalVarName;
std::string ResourceManager::texCoordVarName{};

namespace rc
{
  namespace _details
  {
    void initModel(Model *raw, GLint texID, GLint gpuSlot)
    {
      using RM = ResourceManager;
      if (not RM::texCoordVarName.empty() and texID == -1)
        throw std::runtime_error("Tried to init a model with texture but without a texture ID");
      InitModel(raw, RM::program, RM::vertexVarName.c_str(), RM::normalVarName.c_str(), RM::texCoordVarName.c_str(), gpuSlot, texID);
    }
  }
}

namespace
{
  // source https://learnopengl.com/getting-started/textures
  GLuint addPngTexture(std::string const &path)
  {
    dbg::if_dlevel<dbg::Level::INFO>([](){std::cerr << "Started to load texture" << std::endl;});
    GLuint texture = -1;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width = 0, height = 0, nrChannels = 0;
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
      glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
      glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
      glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
      if (nrChannels == 4)
      {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
      }
      else
      {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
      }
      glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
      dbg::if_dlevel<dbg::Level::INFO>([](){std::cerr << "Failed to load texture" << std::endl;});
    }
    stbi_image_free(data);
    return texture;
  }

  GLuint loadTexture(std::string const &path)
  {
    if (path.back() == 'a')
    {
      GLuint myTex = -1;
      glGenBuffers(1, &myTex);
      LoadTGATextureSimple(path.c_str(), &myTex);
      glBindTexture(GL_TEXTURE_2D, myTex);
      // glUniform1i(glGetUniformLocation(program_color, "texUnit"), 0); // Texture unit 0
      // glActiveTexture(GL_TEXTURE0);
      return myTex;
    }
    else
    {
      return addPngTexture(path);
    }
  }
}
Model *ResourceManager::getModel(std::string const &key, std::string const &path, GLint texID, GLint gpuSlot)
{
  auto &&fullpath = std::string("rc/models/") + path; // MUST BE USED from root folder of project
  Model *model{nullptr};
  if (auto occ = modelsMap.find(key); occ == modelsMap.end())
  {
    if (!(model = LoadModel(fullpath.c_str())))
      throw std::runtime_error("Could not load model " + fullpath);

    modelsMap.emplace(key, model);
    _details::initModel(model, texID, gpuSlot);
  }

  return modelsMap[key].get();
}

Model *ResourceManager::getModel(std::string const &key,
                                 vec3 *vert,
                                 vec3 *norm,
                                 vec2 *texCoords,
                                 vec3 *colors,
                                 GLuint *indices,
                                 int numVert,
                                 int numInd)
{
  Model *model{nullptr};
  if (auto occ = modelsMap.find(key); occ == modelsMap.end())
  {
    if (!(model = LoadDataToModel(vert, norm, texCoords, colors, indices, numVert, numInd)))
      throw std::runtime_error("Could not load model from data");
    else
      modelsMap.emplace(key, model);
    _details::initModel(model);
  }

  return modelsMap[key].get();
}

GLuint ResourceManager::getTexture(std::string const &key, const std::string &path)
{
  auto &&fullpath = std::string("rc/textures/") + path; // MUST BE USED from root folder of project
  GLuint texture;
  if (auto occ = texturesMap.find(key); occ == texturesMap.end())
  {
    if (texture = loadTexture(fullpath); texture == -1)
      throw std::runtime_error("Could not load texture " + fullpath);
    else
      texturesMap.emplace(key, texture);
  }

  return texturesMap[key];
}

TextureDataPtr ResourceManager::getTextureData(std::string const &key, std::string const &path)
{
  auto &&fullpath = std::string("rc/textures/") + path; // MUST BE USED from root folder of project

  TextureData *txtData = (TextureData *)malloc(sizeof(TextureData));
  if (auto occ = texturesDataMap.find(key); occ == texturesDataMap.end())
  {
    if (!(LoadTGATextureData(fullpath.c_str(), txtData)))
      throw std::runtime_error("Could not load texture data " + fullpath);
    else
      texturesDataMap.emplace(key, txtData);
  }

  return texturesDataMap[key].get();
}