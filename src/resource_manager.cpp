#include "resource_manager.hpp"
#include <stdexcept>
#include <string>

#define MODELS_PFX_STR "../rc/models/"
#define MODELS_PFX_LEN 13

Model *ResourceManager::getModel(std::string const &key, std::string const &path)
{
  auto &&fullpath = std::string("rc/models/") + path; // MUST BE USED from root folder of project
  Model *model{nullptr};
  if (auto occ = modelsMap.find(key); occ == modelsMap.end())
  {
    if (!(model = LoadModel(fullpath.c_str())))
      throw std::runtime_error("Could not load model " + fullpath);
    else
      modelsMap.emplace(key, model);
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
  }

  return modelsMap[key].get();
}

GLuint ResourceManager::getTexture(std::string const &key)
{
  return 0;
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