#include "terrain.hpp"
#include "VectorUtils4.h"
#include <math.h>
#include "LoadTGA.h"
#include "../resource_manager.hpp"

namespace scn
{
  namespace
  {
    void addToNormalArray(vec3 *normalarray, vec3 *vertexarray, int previousVertexIndex, int thisVertexIndex, int nextVertexIndex)
    {
      vec3 a = vertexarray[thisVertexIndex] - vertexarray[previousVertexIndex];
      vec3 b = vertexarray[thisVertexIndex] - vertexarray[nextVertexIndex];
      vec3 crossAB = cross(a, b);
      float weight = std::asin(Norm(crossAB) / (Norm(a) * Norm(b)));

      normalarray[thisVertexIndex] -= weight * crossAB;
    }

    /**
     * @brief
     *
     * @param indexArray
     * @param normalArray
     * @param vertexArray
     * @param first   index of node
     * @param second  index of node
     * @param third   index of node
     * @param offset  current index in the array described triangles
     * @param width   width of one line in the array -> jump in the next dimension
     * @param x       x index of the first node of the triangle
     * @param z       y index of the first node of the triangle
     */
    void addTriangle(GLuint *indexArray, vec3 *normalArray, vec3 *vertexArray, int first, int second, int third, int offset, int width, int x, int z)
    {
      indexArray[(x + z * (width - 1)) * 6 + 0 + offset] = first;
      indexArray[(x + z * (width - 1)) * 6 + 1 + offset] = second;
      indexArray[(x + z * (width - 1)) * 6 + 2 + offset] = third;

      addToNormalArray(normalArray, vertexArray, first, second, third);
      addToNormalArray(normalArray, vertexArray, third, first, second);
      addToNormalArray(normalArray, vertexArray, second, third, first);
    }

    Model *generateTerrain(TextureData *tex)
    {
      GLfloat scalingFactor =Terrain::scalingFactor;
      int vertexCount = tex->width * tex->height;
      int triangleCount = (tex->width - 1) * (tex->height - 1) * 2;
      unsigned int x, z;
      vec3 *vertexArray = (vec3 *)malloc(sizeof(GLfloat) * 3 * vertexCount);
      vec3 *normalArray = (vec3 *)malloc(sizeof(GLfloat) * 3 * vertexCount);
      vec2 *texCoordArray = (vec2 *)malloc(sizeof(GLfloat) * 2 * vertexCount);
      GLuint *indexArray = (GLuint *)malloc(sizeof(GLuint) * triangleCount * 3);

      for (x = 0; x < tex->width; x++)
        for (z = 0; z < tex->height; z++)
        {
          // Vertex array. You need to scale this properly
          vertexArray[(x + z * tex->width)].x = x *scalingFactor;
          vertexArray[(x + z * tex->width)].y = (tex->imageData[(x + z * tex->width) * (tex->bpp / 8)] / 10.0 - 20);//*scalingFactor;
          vertexArray[(x + z * tex->width)].z = z *scalingFactor;
          // Normal vectors. You need to calculate these.

          normalArray[(x + z * tex->width)].x = 0.0; // normalvector for this vertex
          normalArray[(x + z * tex->width)].y = 0.0;
          normalArray[(x + z * tex->width)].z = 0.0;

          // Texture coordinates. You may want to scale them.
          texCoordArray[(x + z * tex->width)].x = x*scalingFactor; // * 10; // (float)x / tex->width;
          texCoordArray[(x + z * tex->width)].y = z*scalingFactor; // * 10; // (float)z / tex->height;
        }
      for (x = 0; x < tex->width - 1; x++)
        for (z = 0; z < tex->height - 1; z++)
        {
          addTriangle(indexArray, normalArray, vertexArray, x + z * tex->width, x + (z + 1) * tex->width, x + 1 + z * tex->width, 0, tex->width, x, z);
          addTriangle(indexArray, normalArray, vertexArray, x + 1 + z * tex->width, x + (z + 1) * tex->width, x + 1 + (z + 1) * tex->width, 3, tex->width, x, z);
        }

      // End of terrain generation
      // Create Model and upload to GPU:

      Model *terrain = LoadDataToModel(
          vertexArray,
          normalArray,
          texCoordArray,
          NULL,
          indexArray,
          vertexCount,
          triangleCount * 3);
      if (not terrain)
      {
        free(vertexArray);
        free(indexArray);
        free(normalArray);
        free(texCoordArray);
      }
      return terrain;
    }
  }
}

namespace scn
{
  Terrain::Terrain(std::string const &name, std::string const &path)
      : key{name},
        m_model{nullptr}
  {
    auto &rmgr{ResourceManager::get()};
    TextureData *texp{rmgr.getTextureData(name, path)};
//    m_width = (texp->width-1)*scalingFactor +1;     // last position must not be scaled
 //   m_height = (texp->height-1)*scalingFactor+1;
    m_width = (texp->width)*scalingFactor;     // last position must not be scaled
    m_height = (texp->height)*scalingFactor;
    m_vertices_height=texp->height;
    m_vertices_width=texp->width;
    auto mdl{rmgr.getModel(key, generateTerrain, texp)};
    m_model = Modelv2(mdl);
  }

  Terrain::Terrain(std::string const &path)
      : Terrain(path, path)
  {
  }

  void Terrain::getTriangleVectors(float x_scaled, float z_scaled, vec3 &v1, vec3 &v2, vec3 &startingPoint) const
  {
    auto model{this->model().get()};


    float x = x_scaled/scalingFactor;
    float z = z_scaled/scalingFactor;
   
    float x_difference = x - int(x);// /scalingFactor)*scalingFactor;     
    float z_difference = z - int(z);// /scalingFactor)*scalingFactor;
    vec3 p_bottom_left = model->vertexArray[int(x) + (int(z) + 1) * m_vertices_width];
    vec3 p_top_right = model->vertexArray[((int(x) + 1) + int(z) * m_vertices_width)];
    if (x_difference + z_difference < 1.0)
    {
      startingPoint = model->vertexArray[(int(x) + int(z) * m_vertices_width)]; // p_top_left;
    }
    else
    {
      startingPoint = model->vertexArray[((int(x) + 1) + (int(z) + 1) * m_vertices_width)];
    }
    v1 = (p_bottom_left - startingPoint); // movement on the x axis (top left to bottom left)
    v2 = (p_top_right - startingPoint);   // movement on the z axis (top left to top right)
  }

  float Terrain::computeHeight(float x, float z) const
  {
    float x_difference = x - int(x/scalingFactor)*scalingFactor;
    float z_difference = z - int(z/scalingFactor)*scalingFactor;

    vec3 v1, v2, startingPoint;
    getTriangleVectors(x, z, v1, v2, startingPoint);

    if (x_difference + z_difference < 1.0)
    {
      v1 = v1 * z_difference; // movement on the x axis (top left to bottom left)
      v2 = v2 * x_difference; // movement on the z axis (top left to top right)
    }
    else
    {
      v1 = v1 * (1 - x_difference); // movement on the x axis (top left to bottom left)
      v2 = v2 * (1 - z_difference); // movement on the z axis (top left to top right)
    }
    return (startingPoint + v1 + v2).y;
  }

  float Terrain::nextInsideFieldWidth(float input, float offset) const
  {
    if (input < offset)
      return offset;
    if (input > m_width - offset)
      return m_width - offset;
    return input;
  }

  vec3 Terrain::getNormal(float x, float z) const
  {

    vec3 v1, v2, startingPoint;
    getTriangleVectors(x, z, v1, v2, startingPoint);
    vec3 normal = normalize(CrossProduct(v1, v2));
    if (normal.y < 0)
    {
      return -normal;
    }
    else
    {
      return normal;
    }
  }
}