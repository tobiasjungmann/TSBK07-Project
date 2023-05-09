#pragma once
/*
Todo compute terrain here -> p
return model for main.init()
provide height of terrain at given x & z position
*/

#include "LittleOBJLoader.h"
#include "LoadTGA.h"

namespace scn
{

    class Terrain
    {
    public:
        Terrain()
        {
        }
        Terrain(std::string filename);
        Model *generateTerrain(TextureData *tex);

        /**
         * @brief Returns z coordinate of the terrain at a given point on the generated terrain.
         * identifies corresponding nodes and then computes the vector on the plane to the given x and y coordinates.
         *
         * @param x - Coordinate
         * @param z - Coordiante
         * @return float
         */
        float computeHeight(float x, float z) const;
         vec3 getNormal(float x, float z) const;

        Model *getModel() const;
        float nextInsideFieldWidth(float input) const;

    private:
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
        void addTriangle(GLuint *indexArray, vec3 *normalArray, vec3 *vertexArray, int first, int second, int third, int offset, int width, int x, int z);
        void addToNormalArray(vec3 *normalarray, vec3 *vertexarray, int previousVertexIndex, int thisVertexIndex, int nextVertexIndex);
        void getTriangleVectors(float x, float z, vec3 &v1, vec3 &v2, vec3& startingPoint) const;

        int terrain_width;
        int terrain_height;
        Model *model;
    };
}