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
	
	vec3 Ka;
	vec3 Kd;
	vec3 Ks;

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

typedef struct Model Model;

// Drawing models
void DrawModel(Model *m, GLuint program);
void InitModel(Model *m, GLuint program, const char* vertexVariableName, const char* normalVariableName, const char* texCoordVariableName); // FIXME this overload should no longer be allowed
#ifdef __cplusplus
void InitModel(Model *m, GLuint program, const char* vertexVariableName, const char* normalVariableName, const char* texCoordVariableName, GLint texUnit, GLint texReferenceID);
#endif // __cplusplus
void DrawWireframeModel(Model *m, GLuint program, const char* vertexVariableName, const char* normalVariableName, const char* texCoordVariableName);

// Basic model loading
#define LoadModelPlus LoadModel
Model* LoadModel(const char* name); // Load OBJ as single Model
Model** LoadModelSet(const char* name);  // Multi-part OBJ!

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
	// initialize
	inline void init(GLuint program, const char* vertexVariableName, const char* normalVariableName, const char* texCoordVariableName) {
		InitModel(this, program, vertexVariableName, normalVariableName, texCoordVariableName);
	}
	inline void init(GLuint program, const char* vertexVariableName, const char* normalVariableName, const char* texCoordVariableName, GLint texUnit, GLint texReferenceID) {
		InitModel(this, program, vertexVariableName, normalVariableName, texCoordVariableName, texUnit, texReferenceID);
	}
	// static load 
	inline static Model* load(const char* name) {
		Model* mdl = LoadModel(name);
		if (!mdl)
			throw std::runtime_error("Failed to load model");
		return mdl;
	}
	inline static Model** loadSet(const char* name) {
		Model** mdlp = LoadModelSet(name);
		if (!mdlp)
			throw std::runtime_error("Failed to load model set");
		return mdlp;
	}
	inline static Model* loadFromData(
			vec3 *vertices,
			vec3 *normals,
			vec2 *texCoords,
			vec3 *colors,
			GLuint *indices,
			int numVert,
			int numInd)
		{
			Model* mdl = LoadDataToModel(vertices, normals, texCoords, colors, indices, numVert, numInd);
			if (!mdl)
				throw std::runtime_error("Failed to load model from data");
			return mdl;
		}
	// utilities methods
	inline void reloadData() {ReloadModelData(this);}
	inline void center() {CenterModel(this);}
	inline void scale(float sx, float sy, float sz) {ScaleModel(this, sx, sy, sz);}
	// drawing
	inline void draw(GLuint program) {DrawModel(this, program);}
	inline void drawWireframe(GLuint program, const char* vertexVariableName, const char* normalVariableName, const char* texCoordVariableName)
	{ DrawWireframeModel(this, program, vertexVariableName, normalVariableName, texCoordVariableName); }
	#endif // __cplusplus

} Model;



//#ifdef __cplusplus
//}
///#endif

#endif