#include "resource_manager.hpp"
#include "LoadTGA.h"
#include <cstring>
#include <stdexcept>


#define MODELS_PFX_STR "../rc/models/"
#define MODELS_PFX_LEN 13

Model *ResourceManager::getModel(std::string const &key, const char *path)
{
  size_t len = strlen(path);
  char full_path[MODELS_PFX_LEN + len + 1];
  strcpy(full_path, MODELS_PFX_STR);
  strcat(full_path, path);

  Model *model;
  if (!modelsMap[key])
    if (!(model = LoadModel(full_path)))
      throw std::runtime_error("Could not load model " + std::string(full_path));
    else
      modelsMap[key] = std::move(std::unique_ptr<Model>(model));

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
  Model *model;
  if (!modelsMap[key])
    if (!(model = LoadDataToModel(vert, norm, texCoords, colors, indices, numVert, numInd)))
      throw std::runtime_error("Could not load model from data");
    else
      modelsMap[key] = std::move(std::unique_ptr<Model>(model));

  return modelsMap[key].get();
}
