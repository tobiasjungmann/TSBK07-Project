#pragma once

#include <unordered_map>
#include <memory>
#include <string>
#include "LittleOBJLoader.h"
#include "LoadTGA.h"

class ResourceManager
{
private:
  struct TextureDataDeleter {
    void operator()(TextureData *ptr) {
      FreeTextureData(ptr);
    }
  };

public:
  static ResourceManager &get()
  {
    static ResourceManager self;
    return self;
  }

  /*
   * @brief 
   * 
   *
   * or load it from disk if it doesnt exist already
   *
   * @param key Unique identifier of the Model looked for
   * @param path Suffix to append to ../rc/models/
   * @return Model*
   */
  Model *getModel(std::string const &key, std::string const &path = "");
  Model *getModel(std::string const &key, vec3 *vert, vec3 *norm, vec2 *texCoords, vec3 *colors, GLuint *indices, int numVert, int numInd);

  template <typename Generator, typename... With>
  Model *getModel(std::string const &key, Generator mdlGenerator, With... with);
  inline void freeModel(std::string const &key)
  {
    modelsMap.erase(key);
  }

  GLuint getTexture(std::string const &key);
  inline void freeTexture(std::string const &key)
  {
    texturesMap.erase(key);
  }

  TextureDataPtr getTextureData(std::string const &key, std::string const &path = "");
  inline void freeTextureData(std::string const &key)
  {
    texturesDataMap.erase(key);
  }

  ResourceManager(ResourceManager const &) = delete;
  ResourceManager &operator=(ResourceManager const &) = delete;

private:
  ResourceManager() = default;

private:
  using txtDataUnqPtr_t = std::unique_ptr<TextureData, TextureDataDeleter>;
  std::unordered_map<std::string, std::unique_ptr<Model>> modelsMap;
  std::unordered_map<std::string, GLuint> texturesMap;
  std::unordered_map<std::string, txtDataUnqPtr_t> texturesDataMap;
};

template <typename Generator, typename... With>
inline Model *ResourceManager::getModel(std::string const &key, Generator mdlGenerator, With... with)
{
  Model *model;
  if (auto occ = modelsMap.find(key); occ == modelsMap.end())
  {
    if (!(model = mdlGenerator(with...)))
      throw std::runtime_error("Could not generate model from data");
    else
      modelsMap.emplace(key, model);
  }

  return modelsMap[key].get();
}