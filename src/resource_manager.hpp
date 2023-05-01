#pragma once

#include <unordered_map>
#include <memory>
#include <string>
#include "LittleOBJLoader.h"



class ResourceManager
{
public:
  static ResourceManager& get() {
    static ResourceManager self;
    return self;
  }

  /**
  * @brief Get the Model object identified by the string
    * or load it from disk if it doesnt exist already
  * 
  * @param key Unique identifier of the Model looked for
  * @param path Suffix to append to ../rc/models/
  * @return Model* 
  */
  Model* getModel(std::string const& key, std::string const& path = "");
  Model* getModel(std::string const& key, vec3* vert, vec3* norm, vec2* texCoords, vec3* colors, GLuint *indices, int numVert, int numInd);
  inline void freeModel(std::string const& key) {
    modelsMap.erase(key);
  }

  GLuint getTexture(std::string const& key);
  inline void freeTexture(std::string const& key) {
    texturesMap.erase(key);
  }

  ResourceManager(ResourceManager const&) = delete;
  ResourceManager& operator=(ResourceManager const&) = delete;

private:
  ResourceManager() = default;

private:
  std::unordered_map<std::string, std::unique_ptr<Model>> modelsMap;
  std::unordered_map<std::string, GLuint> texturesMap;
};