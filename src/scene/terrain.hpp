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
        Model *GenerateTerrain(TextureData *tex);
        float computeHeight(float x, float z);

    private:
        void addTriangle(GLuint *indexArray, vec3 *normalArray, vec3 *vertexArray, int first, int second, int third, int offset, int width, int x, int z);
        void addToNormalArray(vec3 *normalarray, vec3 *vertexarray, int previousVertexIndex, int thisVertexIndex, int nextVertexIndex);
        int terrain_width;
        int terrain_height;
        Model * m_terrain;
    };
}