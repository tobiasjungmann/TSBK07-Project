#ifndef loadobj_h
#define loadobj_h

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
#else
	#if defined(_WIN32)
		#include "glew.h"
	#endif
	#include <GL/gl.h>
#endif

#include "VectorUtils4.h"

//#ifdef __cplusplus
//extern "C" {
//#endif

// How many error messages do you want before it stops?
#define NUM_DRAWMODEL_ERROR 8

typedef struct Mtl
{
	char newmtl[255];
	
	vec3 Ke;
	
	union {
		vec3 Ka;
		vec3 ambiantCoeff;
	};
	union {
		vec3 Kd;
		vec3 diffuseness;
	};
	union {
		vec3 Ks;
		vec3 specularity;
	};

	union {
		GLfloat specularExponent, alpha;
	};

	// I have also seen vec3's named Tf and a float called Ni in one file. What is that?
	// Tf = transmission filter
	// Ni = optical_density
	GLfloat Ns, Tr, d; // Tr = 1-d
	int illum;	// illumination model 0..10
	char map_Ka[255], map_Kd[255], map_Ks[255], map_Ke[255], map_Ns[255], map_d[255], map_bump[255];
	
	// NEW for texture support
	GLint texRefID = -1, texidKa, texidKd, texidKs, texidKe, texidNs, texid_d, texid_bump; // References to loaded textures!
	GLint texUnit = -1;
	// refl -type not supported -o for options? A whole bunch.
	// Extensions for physically based rendering not supported
} Mtl, *MtlPtr, **MtlHandle;

typedef struct Model
{
  vec3* vertexArray;
  vec3* normalArray;
  vec2* texCoordArray;
  vec3* colorArray; // Rarely used
  GLuint* indexArray;
  int numVertices;
  int numIndices;
  char externalData;
  
  // Space for saving VBO and VAO IDs
  GLuint vao; // VAO
  GLuint vb, ib, nb, tb; // VBOs

  MtlPtr material;

	#ifdef __cplusplus
	~Model();
	#endif
} Model;

// Basic model loading
#define LoadModelPlus LoadModel
Model* LoadModel(const char* name); // Load OBJ as single Model
Model** LoadModelSet(const char* name);  // Multi-part OBJ!

// Drawing models
void DrawModel(Model *m, GLuint program);
void InitModel(Model *m, GLuint program, const char* vertexVariableName, const char* normalVariableName, const char* texCoordVariableName); // FIXME this overload should no longer be allowed
#ifdef __cplusplus
void InitModel(Model *m, GLuint program, const char* vertexVariableName, const char* normalVariableName, const char* texCoordVariableName, GLint texUnit, GLint texReferenceID);
#endif // __cplusplus
void DrawWireframeModel(Model *m, GLuint program, const char* vertexVariableName, const char* normalVariableName, const char* texCoordVariableName);

// Utility functions that you may need if you want to modify the model.

Model* LoadDataToModel(
			vec3 *vertices,
			vec3 *normals,
			vec2 *texCoords,
			vec3 *colors,
			GLuint *indices,
			int numVert,
			int numInd);
void ReloadModelData(Model *m);

void CenterModel(Model *m);
void ScaleModel(Model *m, float sx, float sy, float sz);
void DisposeModel(Model *m);

void pushTexture(const Model *m, GLuint program, const char* unitName);
#ifdef __cplusplus
void pushTexture(const Model *m, GLuint program, GLint texUnitLoc);
#endif // __cplusplus
//#ifdef __cplusplus
//}
///#endif

#endif