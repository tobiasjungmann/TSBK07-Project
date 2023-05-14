#pragma once

#include <unordered_map>
#include <memory>
#include <string>
#include "LittleOBJLoader.h"
#include "LoadTGA.h"

namespace rc
{
  class ResourceManager;
  namespace _details
  {
    struct TextureDataDeleter;
    struct ModelDeleter;
    void initModel(Model *raw, GLint texID = -1, GLint gpuSlot = 0);
  }

  namespace _details
  {
    struct TextureDataDeleter
    {
      void operator()(TextureData *ptr)
      {
        FreeTextureData(ptr);
      }
    };

    struct ModelDeleter
    {
      void operator()(Model *ptr)
      {
        DisposeModel(ptr);
      }
    };

  }
  class ResourceManager
  {
  private:
    static GLuint program;
    static bool initialized;
    static std::string normalVarName;
    static std::string vertexVarName;
    static std::string texCoordVarName;

    friend void _details::initModel(Model *raw, GLint texID, GLint gpuSlot);

  public:
    static ResourceManager &get()
    {
      static ResourceManager self;
      if (not ResourceManager::initialized)
        throw std::runtime_error("Tried to access ResourceManager without prior initialization");
      return self;
    }

    static void config(GLuint program, std::string vertexVarName, std::string normalVarName, std::string texCoordVarName = "")
    {
      ResourceManager::initialized = true;
      ResourceManager::program = program;
      ResourceManager::vertexVarName = vertexVarName;
      ResourceManager::normalVarName = normalVarName;
      if (not texCoordVarName.empty())
        ResourceManager::texCoordVarName = texCoordVarName;
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
    Model *getModel(std::string const &key, std::string const &path = "", GLint texID = -1, GLint gpuSlot = 0);
    Model *getModel(std::string const &key,
                    vec3 *vert,
                    vec3 *norm,
                    vec2 *texCoords,
                    vec3 *colors,
                    GLuint *indices,
                    int numVert,
                    int numInd);

    template <typename Generator, typename... With>
    Model *getModel(std::string const &key, Generator mdlGenerator, With... with);
    inline void freeModel(std::string const &key)
    {
      modelsMap.erase(key);
    }

    GLuint getTexture(std::string const &key, std::string const &path = "");
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
    /* // BUG singleton with unique ptr preent from freeing models that is no longer used
    // would be better to instead store raw pointer here, then give out shared_ptr when getModel is called
    // and provide a custom deleter that on deletion remove the the resource from this Store.
    */
    using txtDataUnqPtr_t = std::unique_ptr<TextureData, _details::TextureDataDeleter>;
    using modelUnqPtr_t = std::unique_ptr<Model, _details::ModelDeleter>;
    template <typename T>
    using store_t = std::unordered_map<std::string, T>;
    store_t<modelUnqPtr_t> modelsMap;
    store_t<GLuint> texturesMap;
    store_t<txtDataUnqPtr_t> texturesDataMap;
  };

  template <typename Generator, typename... With>
  inline Model *ResourceManager::getModel(std::string const &key, Generator mdlGenerator, With... with)
  {
    Model *model{nullptr};
    if (auto occ = modelsMap.find(key); occ == modelsMap.end())
    {
      if (!(model = mdlGenerator(with...)))
        throw std::runtime_error("Could not generate model from data");

      modelsMap.emplace(key, model);
      _details::initModel(model);
    }

    return modelsMap[key].get();
  }
}