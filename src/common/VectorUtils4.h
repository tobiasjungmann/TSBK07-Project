// VectorUtils4, header only version!
// You must define MAIN in only one file (typically the main
// program) in order to only complile the implementation section once!
// The #define must be BEFORE the "include of this file!

// VectorUtils
// Vector and matrix manipulation library for OpenGL, a package of the most essential functions.
// This is intended as a mini version of GLM.
// Includes:
// - Basic vector operations: Add, subtract, scale, dot product, cross product.
// - Basic matrix operations: Multiply matrix to matrix, matric to vector, transpose.
// - Creation of transformation matrixces: Translation, scaling, rotation.
// - A few more special operations: Orthonormalizaton of a matrix, CrossMatrix,
// - Replacements of some GLU functions: lookAt, frustum, perspective.
// - Inverse and inverse transpose for creating normal matrices.
// Supports both C and C++. The C interface makes it accessible from most languages if desired.

// A note on completeness:
// All operations may not be 100% symmetrical over all types, and some GLSL types are
// missing (like vec2). These will be added if proven important. There are already
// some calls of minor importance (mat3 * mat3, mat3 * vec3) included only for
// symmetry. I don't want the code to grow a lot for such reasons, I want it to be
// compact and to the point.

// Current open design questions:
// Naming conventions: Lower case or capitalized? (Frustum/frustum)
// Prefix for function calls? (The cost would be more typing and making the code harder to read.)
// Add vector operations for vec4? Other *essential* symmetry issues?
// Names for functions when supporting both vec3 and vec4, mat3 and mat4? (vec3Add, vec4Add?)


// History:

// VectorUtils is a small (but growing) math unit by Ingemar Ragnemalm.
// It originated as "geom3d" by Andrew Meggs, but that unit is no more
// than inspiration today. The original VectorUtils(1) was based on it,
// while VectorUtils2 was a rewrite primarily to get rid of the over-use
// of arrays in the original.

// New version 120201:
// Defaults to matrices "by the book". Can also be configured to the flipped
// column-wise matrices that old OpenGL required (and we all hated).
// This is freshly implemented, limited testing, so there can be bugs.
// But it seems to work just fine on my tests with translation, rotations
// and matrix multiplications.

// 1208??: Added lookAt, perspective, frustum
// Also made Transpose do what it should. TransposeRotation is the old function.
// 120913: Fixed perspective. Never trust other's code...
// 120925: Transposing in CrossMatrix
// 121119: Fixed one more glitch in perspective.

// 130227 First draft to a version 3.
// C++ operators if accessed from C++
// Vectors by value when possible. Return values on the stack.
// (Why was this not the case in VectorUtils2? Beause I tried to stay compatible
// with an old C compiler. Older C code generally prefers all non-scalar data by
// reference. But I'd like to move away from that now.)
// Types conform with GLSL as much as possible (vec3, mat4)
// Added some operations for mat3 and vec4, but most of them are more for
// completeness than usefulness; I find vec3's and mat4's to be what I use
// most of the time.
// Also added InvertMat3 and InversTranspose to support creation of normal matrices.
// Marked some calls for removal: CopyVector, TransposeRotation, CopyMatrix.
// 130308: Added InvertMat4 and some more vec3/vec4 operators (+= etc)
// 130922: Fixed a vital bug in CrossMatrix.
// 130924: Fixed a bug in mat3tomat4.
// 131014: Added TransposeMat3 (although I doubt its importance)
// 140213: Corrected mat3tomat4. (Were did the correction in 130924 go?)
// 151210: Added printMat4 and printVec3.
// 160302: Added empty constuctors for vec3 and vec4.
// 170221: Uses _WIN32 instead of WIN32
// 170331: Added stdio.h for printMat4 and printVec3
// 180314: Added some #defines for moving closer to GLSL (dot, cross...).
// 2021-05-15: Constructors for vec3 etc replaced in order to avoid
// problems with some C++ compilers.

// 2022-05-05: Created VectorUtils4, now only a header file!
// You must define MAIN in only one file (typically the main
// program) in order to only complile the implementation section once!
// This means:
// + Only ONE file
// + I can now safely re-introduce the constructors again!
// + I can move closer to GLM and GLSL syntax
// - You must #define MAIN in the main program
// This means that VectorUtils can still be used from C and thereby many other languages,
// while taking more advantage of C++ features when using C++.
// 20220525: Revised perpective() to conform with the reference manual as well as GLM.
// 20230205: Added new helper functions, uploadMat4ToShader etc.
// Made a better #ifdef for handling multiple inclusions.

// You may use VectorUtils as you please. A reference to the origin is appreciated
// but if you grab some snippets from it without reference... no problem.

// VectorUtils4 interface part
// See implementation part for more information

#ifndef VECTORUTILS4
#define VECTORUTILS4

#ifdef __APPLE__
	#define GL_SILENCE_DEPRECATION
	#include <OpenGL/gl3.h>
#else
	#if defined(_WIN32)
		#include "glew.h"
	#endif
	#include <GL/gl.h>
#endif


#ifndef M_PI
#define M_PI           3.14159265358979323846
#endif

#define MAX(a, b) (a) >= (b) ? (a) : (b)
#define MIN(a, b) (a) <= (b) ? (a) : (b)
#define CLAMP(v, m, M) (v) = MAX((m), MIN((v), (M)))

// GLSL-style
// These are already changed, here I define the intermediate type names that I use in some demos.
#define Point3D vec3
#define Matrix3D mat3
#define Matrix4D mat4
#define DotProduct dot
#define CrossProduct cross
#define Normalize normalize
#define Transpose transpose
// Furthermore, SplitVector should be revised to conform with reflect()

// Note 210515: I have removed the constructors from my structs below in order to please
// some compilers. They did work but the compilers were upset about C using these
// without knowing about the constructors, while the C++ code happily used them.
// However, we do not need them; you can initialize with SetVec* instead of
// vec*() and it will work.
// Note 2022: These are now back (and more), which is possible when doing this as a header only unit
// so it works with both C and C++ and still allows the constructors.
	
	typedef struct vec4 vec4;
	typedef struct vec3 vec3;
	
	// vec3 is very useful
	typedef struct vec3
	{
//		GLfloat x, y, z;
		union
		{GLfloat x; GLfloat r;};
		union
		{GLfloat y; GLfloat g;};
		union
		{GLfloat z; GLfloat b;};
		#ifdef __cplusplus
//			vec3(vec4 v);
            constexpr vec3();
			constexpr vec3(GLfloat x2, GLfloat y2, GLfloat z2);
			constexpr vec3(GLfloat x2);
			constexpr vec3(vec4 v);
		#endif
	} vec3, *vec3Ptr;
	
	// vec4 is not as useful. Can be a color with alpha, or a quaternion, but IMHO you
	// rarely need homogenous coordinate vectors on the CPU.
	typedef struct vec4
	{
//		GLfloat x, y, z, w; // w or h
		union
		{GLfloat x; GLfloat r;};
		union
		{GLfloat y; GLfloat g;};
		union
		{GLfloat z; GLfloat b;};
		union
		{GLfloat h; GLfloat w; GLfloat a;};
		#ifdef __cplusplus
            constexpr vec4();
			constexpr vec4(GLfloat x2, GLfloat y2, GLfloat z2, GLfloat w2);
			constexpr vec4(GLfloat xyz, GLfloat w2);
			constexpr vec4(vec3 v, GLfloat w2);
			constexpr vec4(vec3 v);
		#endif
	} vec4, *vec4Ptr;

// vec2 is mostly used for texture cordinates, so I havn't bothered defining any operations for it
	typedef struct vec2
	{
//		GLfloat x, y;
		union
		{GLfloat x; GLfloat s;};
		union
		{GLfloat y; GLfloat t;};
		
		#ifdef __cplusplus
            constexpr vec2();
			constexpr vec2(GLfloat x2, GLfloat y2);
		#endif
	} vec2, *vec2Ptr;
	
	typedef struct mat3 mat3;
	typedef struct mat4
	{
		GLfloat m[16];
		#ifdef __cplusplus
            constexpr mat4();
			constexpr mat4(GLfloat x2);	
			constexpr mat4(GLfloat p0, GLfloat p1, GLfloat p2, GLfloat p3,
				GLfloat p4, GLfloat p5, GLfloat p6, GLfloat p7,
				GLfloat p8, GLfloat p9, GLfloat p10, GLfloat p11, 
				GLfloat p12, GLfloat p13, GLfloat p14, GLfloat p15);
			constexpr mat4(mat3 x);
		#endif
	} mat4;
	typedef struct mat3
	{
		GLfloat m[9];
		#ifdef __cplusplus
			constexpr mat3();
			constexpr mat3(GLfloat x2);
			constexpr mat3(GLfloat p0, GLfloat p1, GLfloat p2,
				GLfloat p3, GLfloat p4, GLfloat p5,
				GLfloat p6, GLfloat p7, GLfloat p8);
			constexpr mat3(mat4 x);
			constexpr mat3(vec3 x1, vec3 x2, vec3 x3);
            /* constexpr mat3();
			constexpr mat3(GLfloat x2);
			constexpr mat3(GLfloat p0, GLfloat p1, GLfloat p2,
				GLfloat p3, GLfloat p4, GLfloat p5,
				GLfloat p6, GLfloat p7, GLfloat p8);
			constexpr mat3(mat4 x);
			constexpr mat3(vec3 x1, vec3 x2, vec3 x3);*/
		#endif
	} mat3;



//#ifdef __cplusplus
//extern "C" {
//#endif

// New better name for SetVector and replacements for constructors
	vec2 SetVec2(GLfloat x, GLfloat y);
	vec3 SetVec3(GLfloat x, GLfloat y, GLfloat z);
	vec4 SetVec4(GLfloat x, GLfloat y, GLfloat z, GLfloat w);

	vec3 SetVector(GLfloat x, GLfloat y, GLfloat z);
	mat3 SetMat3(GLfloat p0, GLfloat p1, GLfloat p2, GLfloat p3, GLfloat p4, GLfloat p5, GLfloat p6, GLfloat p7, GLfloat p8);
	mat4 SetMat4(GLfloat p0, GLfloat p1, GLfloat p2, GLfloat p3,
				GLfloat p4, GLfloat p5, GLfloat p6, GLfloat p7,
				GLfloat p8, GLfloat p9, GLfloat p10, GLfloat p11, 
				GLfloat p12, GLfloat p13, GLfloat p14, GLfloat p15
				);
// Basic vector operations on vec3's. (vec4 not included since I never need them.)
// Note: C++ users can also use operator overloading, defined below.
	vec3 VectorSub(vec3 a, vec3 b);
	vec3 VectorAdd(vec3 a, vec3 b);
	vec3 cross(vec3 a, vec3 b);
	GLfloat dot(vec3 a, vec3 b);
	vec3 ScalarMult(vec3 a, GLfloat s);
	GLfloat Norm(vec3 a);
	vec3 normalize(vec3 a);
	vec3 CalcNormalVector(vec3 a, vec3 b, vec3 c);
	void SplitVector(vec3 v, vec3 n, vec3 *vn, vec3 *vp);

// Matrix operations primarily on 4x4 matrixes!
// Row-wise by default but can be configured to column-wise (see SetTransposed)

	mat4 IdentityMatrix();
	mat4 Rx(GLfloat a);
	mat4 Ry(GLfloat a);
	mat4 Rz(GLfloat a);
	mat4 T(GLfloat tx, GLfloat ty, GLfloat tz);
	mat4 S(GLfloat sx, GLfloat sy, GLfloat sz);
	mat4 Mult(mat4 a, mat4 b); // dest = a * b - rename to MultMat4 considered but I don't like to make the name of the most common operation longer
	// but for symmetry, MultMat4 is made a synonym:
	#define MultMat4 Mult

	vec3 MultVec3(mat4 a, vec3 b); // result = a * b
	vec4 MultVec4(mat4 a, vec4 b);

// Mat3 operations (new)
	mat3 MultMat3(mat3 a, mat3 b); // m = a * b
	vec3 MultMat3Vec3(mat3 a, vec3 b); // result = a * b

	void OrthoNormalizeMatrix(mat4 *R);
	mat4 transpose(mat4 m);
	mat3 TransposeMat3(mat3 m);
	mat4 ArbRotate(vec3 axis, GLfloat fi);
	mat4 CrossMatrix(vec3 a);
	mat4 MatrixAdd(mat4 a, mat4 b);

// Configure, i.e. if you want matrices to be column-wise
	void SetTransposed(char t);

// GLU replacement functions
	mat4 lookAtv(vec3 p, vec3 l, vec3 v);
	mat4 lookAt(GLfloat px, GLfloat py, GLfloat pz, 
			GLfloat lx, GLfloat ly, GLfloat lz,
			GLfloat vx, GLfloat vy, GLfloat vz);
	mat4 perspective(float fovyInDegrees, float aspectRatio,
                      float znear, float zfar);
	mat4 frustum(float left, float right, float bottom, float top,
                  float znear, float zfar);
	mat4 ortho(GLfloat left, GLfloat right, GLfloat bottom,
			GLfloat top, GLfloat near, GLfloat far);

// For creating a normal matrix
	mat3 InvertMat3(mat3 in);
	mat3 InverseTranspose(mat4 in);
	mat4 InvertMat4(mat4 a);

// Simple conversions
	mat3 mat4tomat3(mat4 m);
	mat4 mat3tomat4(mat3 m);
	vec3 vec4tovec3(vec4 v);
	vec4 vec3tovec4(vec3 v);

// Convenient printing calls
	void printMat4(mat4 m);
	void printVec3(vec3 in);

/* Utility functions for easier uploads to shaders with error messages. */
// NEW as prototype 2022, added to VU 2023
	void uploadMat4ToShader(GLuint shader, const char *nameInShader, mat4 m);
	void uploadUniformIntToShader(GLuint shader, const char *nameInShader, GLint i);
	void uploadUniformFloatToShader(GLuint shader, const char *nameInShader, GLfloat f);
	void uploadUniformFloatArrayToShader(GLuint shader, const char *nameInShader, GLfloat *f, int arrayLength);
	void uploadUniformVec3ToShader(GLuint shader, const char *nameInShader, vec3 v);
	void uploadUniformVec3ArrayToShader(GLuint shader, const char *nameInShader, vec3 *a, int arrayLength);
	void bindTextureToTextureUnit(GLuint tex, int unit);

#ifdef __cplusplus
// Convenient overloads for C++, closer to GLSL
	mat3 inverse(mat3 m);
	mat4 inverse(mat4 m);
	mat3 transpose(mat3 m);
	mat4 S(GLfloat s);
	mat4 S(vec3 s);
	mat4 lookAt(vec3 p, vec3 l, vec3 u);
	mat4 zeroOutTranslation(mat4 lhs);
#else
#endif // __cplusplus

//#ifdef __cplusplus
//}
//#endif

#ifdef __cplusplus
#include <iostream>
// Some C++ operator overloads
// Non-member C++ operators!
// New version 2021-05-2x: Constructiors for vec3 etc replaced in order to avoid
// problems with some C++ compilers.

// --- vec3 operations ---
vec3 operator+(const vec3 &a, const vec3 &b); // vec3+vec3;
vec3 operator-(const vec3 &a, const vec3 &b); // vec3-vec3;
vec3 operator-(const vec3 &a);
	// Questionable, not like GLSL
float operator*(const vec3 &a, const vec3 &b); // vec3 dot vec3;
vec3 operator*(const vec3 &b, double a); // vec3 * scalar;
vec3 operator*(double a, const vec3 &b); // scalar * vec3;
vec3 operator/(const vec3 &b, double a); // vec3 / scalar;
void operator+=(vec3 &a, const vec3 &b); // vec3+=vec3;
void operator-=(vec3 &a, const vec3 &b); // vec3-=vec3;
void operator*=(vec3 &a, const float &b); // vec3*=scalar;
void operator/=(vec3 &a, const float &b); // vec3/=scalar;
// --- vec4 operations ---

vec4 operator+(const vec4 &a, const vec4 &b); // vec4+vec4;
vec4 operator-(const vec4 &a, const vec4 &b); // vec4-vec4;
	// Questionable, not like GLSL
float operator*(const vec4 &a, const vec4 &b); // vec4 dot vec4;
vec4 operator*(const vec4 &b, double a); // vec4 * scalar;
vec4 operator*(double a, const vec4 &b); // scalar * vec4;
vec4 operator/(const vec4 &b, double a); // vec4 / scalar;

void operator+=(vec4 &a, const vec4 &b); // vec4+=vec4;
void operator-=(vec4 &a, const vec4 &b); // vec4-=vec4;
void operator*=(vec4 &a, const float &b); // vec4 *= scalar;
void operator/=(vec4 &a, const float &b); // vec4 /= scalar;
// --- Matrix multiplication ---

// mat4 * mat4
mat4 operator*(const mat4 &a, const mat4 &b);
// mat3 * mat3
mat3 operator*(const mat3 &a, const mat3 &b);
// mat4 * vec3
vec3 operator*(const mat4 &a, const vec3 &b);
// mat4 * vec4
vec4 operator*(const mat4 &a, const vec4 &b);
// mat3 * vec3
vec3 operator*(const mat3 &a, const vec3 &b);

// Matrix comparison
bool operator==(const mat4 &a, const mat4 &b);
bool operator!=(const mat4 &a, const mat4 &b);

// display

std::ostream& operator<<(std::ostream& os, const vec3&v);

#include "VectorUtils4.inl"
#endif // __cplusplus
#endif // VECTORUTILS4
