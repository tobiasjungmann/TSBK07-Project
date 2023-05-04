#include "terrain.hpp"
#include "VectorUtils4.h"
#include <math.h>

namespace scn
{

    Terrain::Terrain(std::string filename)
    {
        TextureData tex;
        LoadTGATextureData(filename.c_str(), &tex);
        m_terrain = generateTerrain(&tex);
    }
    void Terrain::addToNormalArray(vec3 *normalarray, vec3 *vertexarray, int previousVertexIndex, int thisVertexIndex, int nextVertexIndex)
    {
        vec3 a = vertexarray[thisVertexIndex] - vertexarray[previousVertexIndex];
        vec3 b = vertexarray[thisVertexIndex] - vertexarray[nextVertexIndex];
        vec3 crossAB = cross(a, b);
        float weight = std::asin(Norm(crossAB) / (Norm(a) * Norm(b)));

        normalarray[thisVertexIndex] -= weight * crossAB;
    }

    void Terrain::addTriangle(GLuint *indexArray, vec3 *normalArray, vec3 *vertexArray, int first, int second, int third, int offset, int width, int x, int z)
    {
        indexArray[(x + z * (width - 1)) * 6 + 0 + offset] = first;
        indexArray[(x + z * (width - 1)) * 6 + 1 + offset] = second;
        indexArray[(x + z * (width - 1)) * 6 + 2 + offset] = third;

        addToNormalArray(normalArray, vertexArray, first, second, third);
        addToNormalArray(normalArray, vertexArray, third, first, second);
        addToNormalArray(normalArray, vertexArray, second, third, first);
    }

    Model *Terrain::generateTerrain(TextureData *tex)
    {
        int vertexCount = tex->width * tex->height;
        terrain_width = tex->width;
        terrain_height = tex->height;
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
                vertexArray[(x + z * tex->width)].x = x / 1.0;
                vertexArray[(x + z * tex->width)].y = tex->imageData[(x + z * tex->width) * (tex->bpp / 8)] / 10.0 - 20;
                vertexArray[(x + z * tex->width)].z = z / 1.0;
                // Normal vectors. You need to calculate these.

                normalArray[(x + z * tex->width)].x = 0.0; // normalvector for this vertex
                normalArray[(x + z * tex->width)].y = 0.0;
                normalArray[(x + z * tex->width)].z = 0.0;

                // Texture coordinates. You may want to scale them.
                texCoordArray[(x + z * tex->width)].x = x; // * 10; // (float)x / tex->width;
                texCoordArray[(x + z * tex->width)].y = z; // * 10; // (float)z / tex->height;
            }
        for (x = 0; x < tex->width - 1; x++)
            for (z = 0; z < tex->height - 1; z++)
            {
                addTriangle(indexArray, normalArray, vertexArray, x + z * tex->width, x + (z + 1) * tex->width, x + 1 + z * tex->width, 0, tex->width, x, z);
                addTriangle(indexArray, normalArray, vertexArray, x + 1 + z * tex->width, x + (z + 1) * tex->width, x + 1 + (z + 1) * tex->width, 3, tex->width, x, z);
            }

        // End of terrain generation

        // Create Model and upload to GPU:

        m_terrain = LoadDataToModel(
            vertexArray,
            normalArray,
            texCoordArray,
            NULL,
            indexArray,
            vertexCount,
            triangleCount * 3);

        return m_terrain;
    }

    float Terrain::computeHeight(float x, float z)
    {
        float x_difference = x - int(x);
        float z_difference = z - int(z);

        vec3 p_bottom_left = m_terrain->vertexArray[int(x) + (int(z) + 1) * terrain_width];
        vec3 p_top_right = m_terrain->vertexArray[((int(x) + 1) + int(z) * terrain_width)];

        vec3 startingPoint;
        vec3 v1, v2;
        if (x_difference + z_difference < 1.0)
        {
            startingPoint = m_terrain->vertexArray[(int(x) + int(z) * terrain_width)]; // p_top_left;

            v1 = (p_bottom_left - startingPoint) * z_difference; // movement on the x axis (top left to bottom left)
            v2 = (p_top_right - startingPoint) * x_difference;   // movement on the z axis (top left to top right)
        }

        else
        {
            startingPoint = m_terrain->vertexArray[((int(x) + 1) + (int(z) + 1) * terrain_width)];

            v1 = (p_bottom_left - startingPoint) * (1 - x_difference); // movement on the x axis (top left to bottom left)
            v2 = (p_top_right - startingPoint) * (1 - z_difference);   // movement on the z axis (top left to top right)
        }

        return (startingPoint + v1 + v2).y;
    }

    Model *Terrain::getModel()
    {
        return m_terrain;
    }
}