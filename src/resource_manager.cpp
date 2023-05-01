#include "resource_manager.hpp"
#include "LoadTGA.h"
#include <stdexcept>
#include <string>


#define MODELS_PFX_STR "../rc/models/"
#define MODELS_PFX_LEN 13

Model* ResourceManager::getModel(std::string const& key, std::string const& path)
{
  auto fullpath = std::string("rc/models/") + path; // MUST BE USED from root folder of project
  Model *model;
  if (! modelsMap[key]) {
    if (!(model = LoadModel(fullpath.c_str())))
      throw std::runtime_error("Could not load model " + fullpath);
    else
      modelsMap[key] = std::move(std::unique_ptr<Model>(model));
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
  Model *model;
  if (! modelsMap[key]) {
    if (!(model = LoadDataToModel(vert, norm, texCoords, colors, indices, numVert, numInd)))
      throw std::runtime_error("Could not load model from data");
    else
      modelsMap[key] = std::move(std::unique_ptr<Model>(model));
  }

  return modelsMap[key].get();
}
