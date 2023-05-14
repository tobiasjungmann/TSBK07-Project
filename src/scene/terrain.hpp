#pragma once
/*
Todo compute terrain here -> p
return model for main.init()
provide height of terrain at given x & z position
*/
#pragma once

#include "LittleOBJLoader.h"
#include "./modelv2.hpp"
#include <string>

struct TextureData;

namespace scn
{

  class Terrain
  {
  private:
  void getTriangleVectors(float x, float z, vec3 &v1, vec3 &v2, vec3 &startingPoint) const;

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
    float nextInsideFieldWidth(float input, float offset) const;

    vec3 getNormal(float x, float z) const;

    inline Modelv2 const& model() const {
      return m_model;
    }

    inline Modelv2& model() {
      return m_model;
    }

    inline int width() const noexcept {
      return m_width;
    }
    inline int height() const noexcept {
      return m_height;
    }

  private:
    const std::string key;
    Modelv2 m_model;
    int m_width;
    int m_height;
  };
}