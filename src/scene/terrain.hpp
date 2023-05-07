/*
Todo compute terrain here -> p
return model for main.init()
provide height of terrain at given x & z position
*/

#include "LittleOBJLoader.h"
#include "./modelv2.hpp"
#include <string>

struct TextureData;

namespace scn
{

  class Terrain
  {
  private:
    static Model* generateTerrain(TextureData *tex);

  public:
    Terrain(std::string const& path);
    Terrain(std::string const& name, std::string const& path);

    /**
     * @brief Returns z coordinate of the terrain at a given point on the generated terrain.
     * identifies corresponding nodes and then computes the vector on the plane to the given x and y coordinates.
     *
     * @param x - Coordinate
     * @param z - Coordiante
     * @return float
     */
    float computeHeight(float x, float z) const;

    inline Modelv2 model() {
      return m_terrain;
    }

  private:
    const std::string key;
    Modelv2 m_terrain;
    int m_width;
    int m_height;
  };
}