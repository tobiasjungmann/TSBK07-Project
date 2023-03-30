
#include "VectorUtils4.h"

#include <math.h>
#include <stdio.h>

#ifdef __cplusplus
// Some C++ operator overloads
// Non-member C++ operators!
// New version 2021-05-2x: Constructiors for vec3 etc replaced in order to avoid
// problems with some C++ compilers.

// --- vec3 operations ---
vec3 operator+(const vec3 &a, const vec3 &b) // vec3+vec3
{
	return SetVector(a.x+b.x, a.y+b.y, a.z+b.z);
}

vec3 operator-(const vec3 &a, const vec3 &b) // vec3-vec3
{
	return SetVector(a.x-b.x, a.y-b.y, a.z-b.z);
}

vec3 operator-(const vec3 &a)
{
		return SetVector(-a.x, -a.y, -a.z);
}

	// Questionable, not like GLSL
float operator*(const vec3 &a, const vec3 &b) // vec3 dot vec3
{
	return (a.x*b.x+ a.y*b.y+ a.z*b.z);
}

vec3 operator*(const vec3 &b, double a) // vec3 * scalar
{
	return SetVector(a*b.x, a*b.y, a*b.z);
}

vec3 operator*(double a, const vec3 &b) // scalar * vec3
{
	return SetVector(a*b.x, a*b.y, a*b.z);
}

vec3 operator/(const vec3 &b, double a) // vec3 / scalar
{
	return SetVector(b.x/a, b.y/a, b.z/a);
}

void operator+=(vec3 &a, const vec3 &b) // vec3+=vec3
{
	a = a + b;
}

void operator-=(vec3 &a, const vec3 &b) // vec3-=vec3
{
	a = a - b;
}

void operator*=(vec3 &a, const float &b) // vec3*=scalar
{
	a = a * b;
}

void operator/=(vec3 &a, const float &b) // vec3/=scalar
{
	a = a / b;
}

// --- vec4 operations ---

vec4 operator+(const vec4 &a, const vec4 &b) // vec4+vec4
{
	return SetVec4(a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w);
}

vec4 operator-(const vec4 &a, const vec4 &b) // vec4-vec4
{
	return SetVec4(a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w);
}

	// Questionable, not like GLSL
float operator*(const vec4 &a, const vec4 &b) // vec4 dot vec4
{
	return (a.x*b.x+ a.y*b.y+ a.z*b.z+ a.w*b.w);
}

vec4 operator*(const vec4 &b, double a) // vec4 * scalar
{
	return SetVec4(a*b.x, a*b.y, a*b.z, a*b.w);
}

vec4 operator*(double a, const vec4 &b) // scalar * vec4
{
	return SetVec4(a*b.x, a*b.y, a*b.z, a*b.w);
}

vec4 operator/(const vec4 &b, double a) // vec4 / scalar
{
	return SetVec4(b.x/a, b.y/a, b.z/a, b.w/a);
}


void operator+=(vec4 &a, const vec4 &b) // vec4+=vec4
{
	a = a + b;
}

void operator-=(vec4 &a, const vec4 &b) // vec4-=vec4
{
	a = a - b;
}

void operator*=(vec4 &a, const float &b) // vec4 *= scalar
{
	a = a * b;
}

void operator/=(vec4 &a, const float &b) // vec4 /= scalar
{
	a = a / b;
}

// --- Matrix multiplication ---

// mat4 * mat4
mat4 operator*(const mat4 &a, const mat4 &b)
{
	return (Mult(a, b));
}

// mat3 * mat3
mat3 operator*(const mat3 &a, const mat3 &b)
{
	return (MultMat3(a, b));
}

// mat4 * vec3
vec3 operator*(const mat4 &a, const vec3 &b)
{
	return MultVec3(a, b); // result = a * b
}

// mat4 * vec4
vec4 operator*(const mat4 &a, const vec4 &b)
{
	return MultVec4(a, b); // result = a * b
}

// mat3 * vec3
vec3 operator*(const mat3 &a, const vec3 &b)
{
	return MultMat3Vec3(a, b); // result = a * b
}

// --- Matrix Comparison ---
bool operator==(const mat4 &a, const mat4 &b) {
	return (a.m[0] == b.m[0] && a.m[1] == b.m[1] && a.m[2] == b.m[2] && a.m[3] == b.m[3] &&
	a.m[4] == b.m[4] && a.m[5] == b.m[5] && a.m[6] == b.m[6] && a.m[7] == b.m[7] &&
	a.m[8] == b.m[8] && a.m[9] == b.m[9] && a.m[10] == b.m[10] && a.m[11] == b.m[11] &&
	a.m[12] == b.m[12] && a.m[13] == b.m[13] && a.m[14] == b.m[14] && a.m[15] == b.m[15]);
}

bool operator!=(const mat4 &a, const mat4 &b) {
	return !(a == b);
}

std::ostream& operator<<(std::ostream& os, const vec3&v){
	os << '(' << v.x << ',' << v.y << ',' << v.z << ')';
	return os;
}

#endif


// VS doesn't define NAN properly
#ifdef _WIN32
    #ifndef NAN
        static const unsigned long __nan[2] = {0xffffffff, 0x7fffffff};
        #define NAN (*(const float *) __nan)
    #endif
#endif


char transposed = 0;

	vec3 SetVector(GLfloat x, GLfloat y, GLfloat z)
	{
		vec3 v;
		
		v.x = x;
		v.y = y;
		v.z = z;
		return v;
	}

// New better name
	vec2 SetVec2(GLfloat x, GLfloat y)
	{
		vec2 v;
		
		v.x = x;
		v.y = y;
		return v;
	}

	vec3 SetVec3(GLfloat x, GLfloat y, GLfloat z)
	{
		vec3 v;
		
		v.x = x;
		v.y = y;
		v.z = z;
		return v;
	}

	vec4 SetVec4(GLfloat x, GLfloat y, GLfloat z, GLfloat w)
	{
		vec4 v;
		
		v.x = x;
		v.y = y;
		v.z = z;
		v.w = w;
		return v;
	}

// Modern C doesn't need this, but Visual Studio insists on old-fashioned C and needs this.
	mat3 SetMat3(GLfloat p0, GLfloat p1, GLfloat p2, GLfloat p3, GLfloat p4, GLfloat p5, GLfloat p6, GLfloat p7, GLfloat p8)
	{
		mat3 m;
		m.m[0] = p0;
		m.m[1] = p1;
		m.m[2] = p2;
		m.m[3] = p3;
		m.m[4] = p4;
		m.m[5] = p5;
		m.m[6] = p6;
		m.m[7] = p7;
		m.m[8] = p8;
		return m;
	}

// Like above; Modern C doesn't need this, but Visual Studio insists on old-fashioned C and needs this.
	mat4 SetMat4(GLfloat p0, GLfloat p1, GLfloat p2, GLfloat p3,
				GLfloat p4, GLfloat p5, GLfloat p6, GLfloat p7,
				GLfloat p8, GLfloat p9, GLfloat p10, GLfloat p11, 
				GLfloat p12, GLfloat p13, GLfloat p14, GLfloat p15
				)
	{
		return {p0,
		p1,
		p2,
		p3,
		p4,
		p5,
		p6,
		p7,
		p8,
		p9,
		p10,
		p11,
		p12,
		p13,
		p14,
		p15};
	}


	// vec3 operations
	// vec4 operations can easily be added but I havn't seen much need for them.
	// Some are defined as C++ operators though.

	vec3 VectorSub(vec3 a, vec3 b)
	{
		vec3 result;
		
		result.x = a.x - b.x;
		result.y = a.y - b.y;
		result.z = a.z - b.z;
		return result;
	}
	
	vec3 VectorAdd(vec3 a, vec3 b)
	{
		vec3 result;
		
		result.x = a.x + b.x;
		result.y = a.y + b.y;
		result.z = a.z + b.z;
		return result;
	}

	vec3 cross(vec3 a, vec3 b)
	{
		vec3 result;

		result.x = a.y*b.z - a.z*b.y;
		result.y = a.z*b.x - a.x*b.z;
		result.z = a.x*b.y - a.y*b.x;
		
		return result;
	}

	GLfloat dot(vec3 a, vec3 b)
	{
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}

	vec3 ScalarMult(vec3 a, GLfloat s)
	{
		vec3 result;
		
		result.x = a.x * s;
		result.y = a.y * s;
		result.z = a.z * s;
		
		return result;
	}

	GLfloat Norm(vec3 a)
	{
		GLfloat result;

		result = (GLfloat)sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
		return result;
	}

	vec3 normalize(vec3 a)
	{
		GLfloat norm;
		vec3 result;

		norm = (GLfloat)sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
		result.x = a.x / norm;
		result.y = a.y / norm;
		result.z = a.z / norm;
		return result;
	}

	vec3 CalcNormalVector(vec3 a, vec3 b, vec3 c)
	{
		vec3 n;

		n = cross(VectorSub(a, b), VectorSub(a, c));
		n = ScalarMult(n, 1/Norm(n));
		
		return n;
	}

// Splits v into vn (parallell to n) and vp (perpendicular). Does not demand n to be normalized.
	void SplitVector(vec3 v, vec3 n, vec3 *vn, vec3 *vp)
	{
		GLfloat nlen;
		GLfloat nlen2;

		nlen = DotProduct(v, n);
		nlen2 = n.x*n.x+n.y*n.y+n.z*n.z; // Squared length
		if (nlen2 == 0)
		{
			*vp = v;
			*vn = SetVector(0, 0, 0);
		}
		else
		{
			*vn = ScalarMult(n, nlen/nlen2);
			*vp = VectorSub(v, *vn);
		}
	}

// Matrix operations primarily on 4x4 matrixes!
// Row-wise by default but can be configured to column-wise (see SetTransposed)

	mat4 IdentityMatrix()
	{
		return {
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		};
	}

	mat4 Rx(GLfloat a)
	{
		mat4 m;
		m = IdentityMatrix();
		m.m[5] = (GLfloat)cos(a);
		if (transposed)
			m.m[9] = (GLfloat)-sin(a);
		else
			m.m[9] = (GLfloat)sin(a);
		m.m[6] = -m.m[9]; //sin(a);
		m.m[10] = m.m[5]; //cos(a);
		return m;
	}

	mat4 Ry(GLfloat a)
	{
		mat4 m;
		m = IdentityMatrix();
		m.m[0] = (GLfloat)cos(a);
		if (transposed)
			m.m[8] = (GLfloat)sin(a); // Was flipped
		else
			m.m[8] = (GLfloat)-sin(a);
		m.m[2] = -m.m[8]; //sin(a);
		m.m[10] = m.m[0]; //cos(a);
		return m;
	}

	mat4 Rz(GLfloat a)
	{
		mat4 m;
		m = IdentityMatrix();
		m.m[0] = (GLfloat)cos(a);
		if (transposed)
			m.m[4] = (GLfloat)-sin(a);
		else
			m.m[4] = (GLfloat)sin(a);
		m.m[1] = -m.m[4]; //sin(a);
		m.m[5] = m.m[0]; //cos(a);
		return m;
	}

	mat4 T(GLfloat tx, GLfloat ty, GLfloat tz)
	{
		mat4 m;
		m = IdentityMatrix();
		if (transposed)
		{
			m.m[12] = tx;
			m.m[13] = ty;
			m.m[14] = tz;
		}
		else
		{
			m.m[3] = tx;
			m.m[7] = ty;
			m.m[11] = tz;
		}
		return m;
	}

	mat4 S(GLfloat sx, GLfloat sy, GLfloat sz)
	{
		mat4 m;
		m = IdentityMatrix();
		m.m[0] = sx;
		m.m[5] = sy;
		m.m[10] = sz;
		return m;
	}

	mat4 Mult(mat4 a, mat4 b) // m = a * b
	{
		mat4 m;

		int x, y;
		for (x = 0; x <= 3; x++)
			for (y = 0; y <= 3; y++)
				if (transposed)
					m.m[x*4 + y] =	a.m[y+4*0] * b.m[0+4*x] +
								a.m[y+4*1] * b.m[1+4*x] +
								a.m[y+4*2] * b.m[2+4*x] +
								a.m[y+4*3] * b.m[3+4*x];
				else
					m.m[y*4 + x] =	a.m[y*4+0] * b.m[0*4+x] +
								a.m[y*4+1] * b.m[1*4+x] +
								a.m[y*4+2] * b.m[2*4+x] +
								a.m[y*4+3] * b.m[3*4+x];

		return m;
	}

//	mat4 Mult(mat4 a, mat4 b) // m = a * b
//	{
//		mat4 m;

//		if (transposed)
//		{
//			a = transpose(a);
//			b = transpose(b);
//		}

//		int x, y;
//		for (x = 0; x <= 3; x++)
//			for (y = 0; y <= 3; y++)
//					m.m[y*4 + x] =	a.m[y*4+0] * b.m[0*4+x] +
//								a.m[y*4+1] * b.m[1*4+x] +
//								a.m[y*4+2] * b.m[2*4+x] +
//								a.m[y*4+3] * b.m[3*4+x];

//		if (transposed)
//			m = transpose(m);

//		return m;
//	}


	// Ej testad!
	mat3 MultMat3(mat3 a, mat3 b) // m = a * b
	{
		mat3 m;

		int x, y;
		for (x = 0; x <= 2; x++)
			for (y = 0; y <= 2; y++)
				if (transposed)
					m.m[x*3 + y] =	a.m[y+3*0] * b.m[0+3*x] +
								a.m[y+3*1] * b.m[1+3*x] +
								a.m[y+3*2] * b.m[2+3*x];
				else
					m.m[y*3 + x] =	a.m[y*3+0] * b.m[0*3+x] +
								a.m[y*3+1] * b.m[1*3+x] +
								a.m[y*3+2] * b.m[2*3+x];

		return m;
	}

	// mat4 * vec3
	// The missing homogenous coordinate is implicitly set to 1.
	vec3 MultVec3(mat4 a, vec3 b) // result = a * b
	{
		vec3 r;

		if (!transposed)
		{
			r.x = a.m[0]*b.x + a.m[1]*b.y + a.m[2]*b.z + a.m[3];
			r.y = a.m[4]*b.x + a.m[5]*b.y + a.m[6]*b.z + a.m[7];
			r.z = a.m[8]*b.x + a.m[9]*b.y + a.m[10]*b.z + a.m[11];
		}
		else
		{
			r.x = a.m[0]*b.x + a.m[4]*b.y + a.m[8]*b.z + a.m[12];
			r.y = a.m[1]*b.x + a.m[5]*b.y + a.m[9]*b.z + a.m[13];
			r.z = a.m[2]*b.x + a.m[6]*b.y + a.m[10]*b.z + a.m[14];
		}

		return r;
	}

	// mat3 * vec3
	vec3 MultMat3Vec3(mat3 a, vec3 b) // result = a * b
	{
		vec3 r;
		
		if (!transposed)
		{
			r.x = a.m[0]*b.x + a.m[1]*b.y + a.m[2]*b.z;
			r.y = a.m[3]*b.x + a.m[4]*b.y + a.m[5]*b.z;
			r.z = a.m[6]*b.x + a.m[7]*b.y + a.m[8]*b.z;
		}
		else
		{
			r.x = a.m[0]*b.x + a.m[3]*b.y + a.m[6]*b.z;
			r.y = a.m[1]*b.x + a.m[4]*b.y + a.m[7]*b.z;
			r.z = a.m[2]*b.x + a.m[5]*b.y + a.m[8]*b.z;
		}
		
		return r;
	}

	// mat4 * vec4
	vec4 MultVec4(mat4 a, vec4 b) // result = a * b
	{
		vec4 r;

		if (!transposed)
		{
			r.x = a.m[0]*b.x + a.m[1]*b.y + a.m[2]*b.z + a.m[3]*b.w;
			r.y = a.m[4]*b.x + a.m[5]*b.y + a.m[6]*b.z + a.m[7]*b.w;
			r.z = a.m[8]*b.x + a.m[9]*b.y + a.m[10]*b.z + a.m[11]*b.w;
			r.w = a.m[12]*b.x + a.m[13]*b.y + a.m[14]*b.z + a.m[15]*b.w;
		}
		else
		{
			r.x = a.m[0]*b.x + a.m[4]*b.y + a.m[8]*b.z + a.m[12]*b.w;
			r.y = a.m[1]*b.x + a.m[5]*b.y + a.m[9]*b.z + a.m[13]*b.w;
			r.z = a.m[2]*b.x + a.m[6]*b.y + a.m[10]*b.z + a.m[14]*b.w;
			r.w = a.m[3]*b.x + a.m[7]*b.y + a.m[11]*b.z + a.m[15]*b.w;
		}

		return r;
	}


// Unnecessary
// Will probably be removed
//	void CopyMatrix(GLfloat *src, GLfloat *dest)
//	{
//		int i;
//		for (i = 0; i <= 15; i++)
//			dest[i] = src[i];
//	}


// Added for lab 3 (TSBK03)

	// Orthonormalization of Matrix4D. Assumes rotation only, translation/projection ignored
	void OrthoNormalizeMatrix(mat4 *R)
	{
		vec3 x, y, z;

		if (transposed)
		{
			x = SetVector(R->m[0], R->m[1], R->m[2]);
			y = SetVector(R->m[4], R->m[5], R->m[6]);
//		SetVector(R[8], R[9], R[10], &z);
			// Kryssa fram ur varandra
			// Normera
			z = CrossProduct(x, y);
			z = Normalize(z);
			x = Normalize(x);
			y = CrossProduct(z, x);
			R->m[0] = x.x;
			R->m[1] = x.y;
			R->m[2] = x.z;
			R->m[4] = y.x;
			R->m[5] = y.y;
			R->m[6] = y.z;
			R->m[8] = z.x;
			R->m[9] = z.y;
			R->m[10] = z.z;

			R->m[3] = 0.0;
			R->m[7] = 0.0;
			R->m[11] = 0.0;
			R->m[12] = 0.0;
			R->m[13] = 0.0;
			R->m[14] = 0.0;
			R->m[15] = 1.0;
		}
		else
		{
		// NOT TESTED
			x = SetVector(R->m[0], R->m[4], R->m[8]);
			y = SetVector(R->m[1], R->m[5], R->m[9]);
//		SetVector(R[2], R[6], R[10], &z);
			// Kryssa fram ur varandra
			// Normera
			z = CrossProduct(x, y);
			z = Normalize(z);
			x = Normalize(x);
			y = CrossProduct(z, x);
			R->m[0] = x.x;
			R->m[4] = x.y;
			R->m[8] = x.z;
			R->m[1] = y.x;
			R->m[5] = y.y;
			R->m[9] = y.z;
			R->m[2] = z.x;
			R->m[6] = z.y;
			R->m[10] = z.z;

			R->m[3] = 0.0;
			R->m[7] = 0.0;
			R->m[11] = 0.0;
			R->m[12] = 0.0;
			R->m[13] = 0.0;
			R->m[14] = 0.0;
			R->m[15] = 1.0;
		}
	}


// Commented out since I plan to remove it if I can't see a good reason to keep it.
//	// Only transposes rotation part.
//	mat4 TransposeRotation(mat4 m)
//	{
//		mat4 a;
//		
//		a.m[0] = m.m[0]; a.m[4] = m.m[1]; a.m[8] = m.m[2];      a.m[12] = m.m[12];
//		a.m[1] = m.m[4]; a.m[5] = m.m[5]; a.m[9] = m.m[6];      a.m[13] = m.m[13];
//		a.m[2] = m.m[8]; a.m[6] = m.m[9]; a.m[10] = m.m[10];    a.m[14] = m.m[14];
//		a.m[3] = m.m[3]; a.m[7] = m.m[7]; a.m[11] = m.m[11];    a.m[15] = m.m[15];
//		
//		return a;
//	}

	// Complete transpose!
	mat4 transpose(mat4 m)
	{
		mat4 a;
		
		a.m[0] = m.m[0]; a.m[4] = m.m[1]; a.m[8] = m.m[2];      a.m[12] = m.m[3];
		a.m[1] = m.m[4]; a.m[5] = m.m[5]; a.m[9] = m.m[6];      a.m[13] = m.m[7];
		a.m[2] = m.m[8]; a.m[6] = m.m[9]; a.m[10] = m.m[10];    a.m[14] = m.m[11];
		a.m[3] = m.m[12]; a.m[7] = m.m[13]; a.m[11] = m.m[14];    a.m[15] = m.m[15];
		
		return a;
	}

	// Complete transpose!
	mat3 TransposeMat3(mat3 m)
	{
		mat3 a;
		
		a.m[0] = m.m[0]; a.m[3] = m.m[1]; a.m[6] = m.m[2];
		a.m[1] = m.m[3]; a.m[4] = m.m[4]; a.m[7] = m.m[5];
		a.m[2] = m.m[6]; a.m[5] = m.m[7]; a.m[8] = m.m[8];
		
		return a;
	}

// Rotation around arbitrary axis (rotation only)
mat4 ArbRotate(vec3 axis, GLfloat fi)
{
	vec3 x, y, z;
	mat4 R, Rt, Raxel, m;

// Check if parallel to Z
	if (axis.x < 0.0000001) // Below some small value
	if (axis.x > -0.0000001)
	if (axis.y < 0.0000001)
	if (axis.y > -0.0000001)
	{
		if (axis.z > 0)
		{
			m = Rz(fi);
			return m;
		}
		else
		{
			m = Rz(-fi);
			return m;
		}
	}

	x = Normalize(axis);
	z = SetVector(0,0,1); // Temp z
	y = Normalize(CrossProduct(z, x)); // y' = z^ x x'
	z = CrossProduct(x, y); // z' = x x y

	if (transposed)
	{
		R.m[0] = x.x; R.m[4] = x.y; R.m[8] = x.z;  R.m[12] = 0.0;
		R.m[1] = y.x; R.m[5] = y.y; R.m[9] = y.z;  R.m[13] = 0.0;
		R.m[2] = z.x; R.m[6] = z.y; R.m[10] = z.z;  R.m[14] = 0.0;

		R.m[3] = 0.0; R.m[7] = 0.0; R.m[11] = 0.0;  R.m[15] = 1.0;
	}
	else
	{
		R.m[0] = x.x; R.m[1] = x.y; R.m[2] = x.z;  R.m[3] = 0.0;
		R.m[4] = y.x; R.m[5] = y.y; R.m[6] = y.z;  R.m[7] = 0.0;
		R.m[8] = z.x; R.m[9] = z.y; R.m[10] = z.z;  R.m[11] = 0.0;

		R.m[12] = 0.0; R.m[13] = 0.0; R.m[14] = 0.0;  R.m[15] = 1.0;
	}

	Rt = Transpose(R); // Transpose = Invert -> felet ej i Transpose, och det �r en ortonormal matris

	Raxel = Rx(fi); // Rotate around x axis

	// m := Rt * Rx * R
	m = Mult(Mult(Rt, Raxel), R);
	
	return m;
}


// Not tested much
mat4 CrossMatrix(vec3 a) // Matrix for cross product
{
	mat4 m;
	
	if (transposed)
	{
		m.m[0] =    0; m.m[4] =-a.z; m.m[8] = a.y; m.m[12] = 0.0;
		m.m[1] = a.z; m.m[5] =    0; m.m[9] =-a.x; m.m[13] = 0.0;
		m.m[2] =-a.y; m.m[6] = a.x; m.m[10]=    0; m.m[14] = 0.0;
		m.m[3] =  0.0; m.m[7] =  0.0; m.m[11]=  0.0; m.m[15] = 0.0;
		// NOTE! 0.0 in the homogous coordinate. Thus, the matrix can
		// not be generally used, but is fine for matrix differentials
	}
	else
	{
		m.m[0] =    0; m.m[1] =-a.z; m.m[2] = a.y; m.m[3] = 0.0;
		m.m[4] = a.z; m.m[5] =    0; m.m[6] =-a.x; m.m[7] = 0.0;
		m.m[8] =-a.y; m.m[9] = a.x; m.m[10]=    0; m.m[11] = 0.0;
		m.m[12] =  0.0; m.m[13] =  0.0; m.m[14]=  0.0; m.m[15] = 0.0;
		// NOTE! 0.0 in the homogous coordinate. Thus, the matrix can
		// not be generally used, but is fine for matrix differentials
	}
	
	return m;
}

mat4 MatrixAdd(mat4 a, mat4 b)
{
	mat4 dest;
	
	int i;
	for (i = 0; i < 16; i++)
		dest.m[i] = a.m[i] + b.m[i];
	
	return dest;
}

// 0 = row-wise defined matrices
// 1 = column-wise
void SetTransposed(char t)
{
	transposed = t;
}


// Build standard matrices

mat4 lookAtv(vec3 p, vec3 l, vec3 v)
{
	vec3 n, u;
	mat4 rot, trans;

	n = Normalize(VectorSub(p, l));
	u = Normalize(CrossProduct(v, n));
	v = CrossProduct(n, u);

	if (transposed)
	rot = SetMat4(u.x, v.x, n.x, 0,
                  u.y, v.y, n.y, 0,
                  u.z, v.z, n.z, 0,
                  0,   0,   0,   1);
	else
	rot = SetMat4(u.x, u.y, u.z, 0,
                  v.x, v.y, v.z, 0,
                  n.x, n.y, n.z, 0,
                  0,   0,   0,   1);
	trans = T(-p.x, -p.y, -p.z);
	mat4 m = Mult(rot, trans);
	return m;
}

mat4 lookAt(GLfloat px, GLfloat py, GLfloat pz, 
			GLfloat lx, GLfloat ly, GLfloat lz,
			GLfloat vx, GLfloat vy, GLfloat vz)
{
	vec3 p, l, v;
	
	p = SetVector(px, py, pz);
	l = SetVector(lx, ly, lz);
	v = SetVector(vx, vy, vz);
	
	return lookAtv(p, l, v);
}

// From http://www.opengl.org/wiki/GluPerspective_code
// Changed names and parameter order to conform with VU style
// Rewritten 120913 because it was all wrong...

// Creates a projection matrix like gluPerspective or glFrustum.
// Upload to your shader as usual.
// Error fixed 20220525: 180, not 360!
// Also made it conform to the reference manual.
mat4 perspective(float fovyInDegrees, float aspectRatio,
                      float znear, float zfar)
{
	float f = 1/tan(fovyInDegrees / 2);
	mat4 m = SetMat4(f/aspectRatio, 0, 0, 0,
					0, f, 0, 0,
					0, 0, (zfar+znear)/(znear-zfar), 2*zfar*znear/(znear-zfar),
					0, 0, -1, 0);
	if (transposed)
		m = transpose(m);
	return m;
}

mat4 frustum(float left, float right, float bottom, float top,
                  float znear, float zfar)
{
    float temp, temp2, temp3, temp4;
    mat4 matrix;
    
    temp = 2.0f * znear;
    temp2 = right - left;
    temp3 = top - bottom;
    temp4 = zfar - znear;
    matrix.m[0] = temp / temp2; // 2*near/(right-left)
    matrix.m[1] = 0.0;
    matrix.m[2] = 0.0;
    matrix.m[3] = 0.0;
    matrix.m[4] = 0.0;
    matrix.m[5] = temp / temp3; // 2*near/(top - bottom)
    matrix.m[6] = 0.0;
    matrix.m[7] = 0.0;
    matrix.m[8] = (right + left) / temp2; // A = r+l / r-l
    matrix.m[9] = (top + bottom) / temp3; // B = t+b / t-b
    matrix.m[10] = (-zfar - znear) / temp4; // C = -(f+n) / f-n
    matrix.m[11] = -1.0;
    matrix.m[12] = 0.0;
    matrix.m[13] = 0.0;
    matrix.m[14] = (-temp * zfar) / temp4; // D = -2fn / f-n
    matrix.m[15] = 0.0;
    
    if (!transposed)
    	matrix = Transpose(matrix);
    
    return matrix;
}

// Not tested!
mat4 ortho(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat near, GLfloat far)
{
        float a = 2.0f / (right - left);
        float b = 2.0f / (top - bottom);
        float c = -2.0f / (far - near);

        float tx = - (right + left)/(right - left);
        float ty = - (top + bottom)/(top - bottom);
        float tz = - (far + near)/(far - near);

        mat4 o = SetMat4(
            a, 0, 0, tx,
            0, b, 0, ty,
            0, 0, c, tz,
            0, 0, 0, 1);

        if (transposed)
   			o = Transpose(o);

        return o;
}

// The code below is based on code from:
// http://www.dr-lex.be/random/matrix_inv.html

// Inverts mat3 (row-wise matrix)
// (For a more general inverse, try a gaussian elimination.)
mat3 InvertMat3(mat3 in)
{
	float a11, a12, a13, a21, a22, a23, a31, a32, a33;
	mat3 out, nanout;
	float DET;
	
	// Copying to internal variables both clarify the code and
	// buffers data so the output may be identical to the input!
	a11 = in.m[0];
	a12 = in.m[1];
	a13 = in.m[2];
	a21 = in.m[3];
	a22 = in.m[4];
	a23 = in.m[5];
	a31 = in.m[6];
	a32 = in.m[7];
	a33 = in.m[8];
	DET = a11*(a33*a22-a32*a23)-a21*(a33*a12-a32*a13)+a31*(a23*a12-a22*a13);
	if (DET != 0)
	{
		out.m[0] = (a33*a22-a32*a23)/DET;
		out.m[1] = -(a33*a12-a32*a13)/DET;
		out.m[2] = (a23*a12-a22*a13)/DET;
		out.m[3] = -(a33*a21-a31*a23)/DET;
		out.m[4] = (a33*a11-a31*a13)/DET;
		out.m[5] = -(a23*a11-a21*a13)/DET;
		out.m[6] = (a32*a21-a31*a22)/DET;
		out.m[7] = -(a32*a11-a31*a12)/DET;
		out.m[8] = (a22*a11-a21*a12)/DET;
	}
	else
	{
		nanout = SetMat3(NAN, NAN, NAN,
								NAN, NAN, NAN,
								NAN, NAN, NAN);
		out = nanout;
	}
	
	return out;
}

// For making a normal matrix from a model-to-view matrix
// Takes a mat4 in, ignores 4th row/column (should just be translations),
// inverts as mat3 (row-wise matrix) and returns the transpose
mat3 InverseTranspose(mat4 in)
{
	float a11, a12, a13, a21, a22, a23, a31, a32, a33;
	mat3 out, nanout;
	float DET;
	
	// Copying to internal variables
	a11 = in.m[0];
	a12 = in.m[1];
	a13 = in.m[2];
	a21 = in.m[4];
	a22 = in.m[5];
	a23 = in.m[6];
	a31 = in.m[8];
	a32 = in.m[9];
	a33 = in.m[10];
	DET = a11*(a33*a22-a32*a23)-a21*(a33*a12-a32*a13)+a31*(a23*a12-a22*a13);
	if (DET != 0)
	{
		out.m[0] = (a33*a22-a32*a23)/DET;
		out.m[3] = -(a33*a12-a32*a13)/DET;
		out.m[6] = (a23*a12-a22*a13)/DET;
		out.m[1] = -(a33*a21-a31*a23)/DET;
		out.m[4] = (a33*a11-a31*a13)/DET;
		out.m[7] = -(a23*a11-a21*a13)/DET;
		out.m[2] = (a32*a21-a31*a22)/DET;
		out.m[5] = -(a32*a11-a31*a12)/DET;
		out.m[8] = (a22*a11-a21*a12)/DET;
	}
	else
	{
		nanout = SetMat3(NAN, NAN, NAN,
								NAN, NAN, NAN,
								NAN, NAN, NAN);
		out = nanout;
	}

	return out;
}


// Simple conversions
mat3 mat4tomat3(mat4 m)
{
	mat3 result;
	
	result.m[0] = m.m[0];
	result.m[1] = m.m[1];
	result.m[2] = m.m[2];
	result.m[3] = m.m[4];
	result.m[4] = m.m[5];
	result.m[5] = m.m[6];
	result.m[6] = m.m[8];
	result.m[7] = m.m[9];
	result.m[8] = m.m[10];
	return result;
}

mat4 mat3tomat4(mat3 m)
{
	mat4 result;
	
	result.m[0] = m.m[0];
	result.m[1] = m.m[1];
	result.m[2] = m.m[2];
	result.m[3] = 0;
	result.m[4] = m.m[3];
	result.m[5] = m.m[4];
	result.m[6] = m.m[5];
	result.m[7] = 0;
	result.m[8] = m.m[6];
	result.m[9] = m.m[7];
	result.m[10] = m.m[8];
	result.m[11] = 0;

	result.m[12] = 0;
	result.m[13] = 0;
	result.m[14] = 0;
	result.m[15] = 1;
	return result;
}

vec3 vec4tovec3(vec4 v)
{
	vec3 result;
	result.x = v.x;
	result.y = v.y;
	result.z = v.z;
	return result;
}

vec4 vec3tovec4(vec3 v)
{
	vec4 result;
	result.x = v.x;
	result.y = v.y;
	result.z = v.z;
	result.w = 1;
	return result;
}


// Stol... I mean adapted from glMatrix (WebGL math unit). Almost no
// changes despite changing language! But I just might replace it with
// a gaussian elimination some time.
mat4 InvertMat4(mat4 a)
{
   mat4 b;
     
   float c=a.m[0],d=a.m[1],e=a.m[2],g=a.m[3],
	f=a.m[4],h=a.m[5],i=a.m[6],j=a.m[7],
	k=a.m[8],l=a.m[9],o=a.m[10],m=a.m[11],
	n=a.m[12],p=a.m[13],r=a.m[14],s=a.m[15],
	A=c*h-d*f,
	B=c*i-e*f,
	t=c*j-g*f,
	u=d*i-e*h,
	v=d*j-g*h,
	w=e*j-g*i,
	x=k*p-l*n,
	y=k*r-o*n,
	z=k*s-m*n,
	C=l*r-o*p,
	D=l*s-m*p,
	E=o*s-m*r,
	q=1/(A*E-B*D+t*C+u*z-v*y+w*x);
	b.m[0]=(h*E-i*D+j*C)*q;
	b.m[1]=(-d*E+e*D-g*C)*q;
	b.m[2]=(p*w-r*v+s*u)*q;
	b.m[3]=(-l*w+o*v-m*u)*q;
	b.m[4]=(-f*E+i*z-j*y)*q;
	b.m[5]=(c*E-e*z+g*y)*q;
	b.m[6]=(-n*w+r*t-s*B)*q;
	b.m[7]=(k*w-o*t+m*B)*q;
	b.m[8]=(f*D-h*z+j*x)*q;
	b.m[9]=(-c*D+d*z-g*x)*q;
	b.m[10]=(n*v-p*t+s*A)*q;
	b.m[11]=(-k*v+l*t-m*A)*q;
	b.m[12]=(-f*C+h*y-i*x)*q;
	b.m[13]=(c*C-d*y+e*x)*q;
	b.m[14]=(-n*u+p*B-r*A)*q;
	b.m[15]=(k*u-l*B+o*A)*q;
	return b;
};


// Two convenient printing functions suggested by Christian Luckey 2015.
// Added printMat3 2019.
void printMat4(mat4 m)
{
	unsigned int i;
	printf(" ---------------------------------------------------------------\n");
	for (i = 0; i < 4; i++)
	{
		int n = i * 4;
		printf("| %11.5f\t| %11.5f\t| %11.5f\t| %11.5f\t|\n",
			m.m[n], m.m[n+1], m.m[n+2], m.m[n+3]);
	}
	printf(" ---------------------------------------------------------------\n");
}

void printMat3(mat3 m)
{
	unsigned int i;
	printf(" ---------------------------------------------------------------\n");
	for (i = 0; i < 3; i++)
	{
		int n = i * 3;
		printf("| %11.5f\t| %11.5f\t| %11.5f\t| \n",
			m.m[n], m.m[n+1], m.m[n+2]);
	}
	printf(" ---------------------------------------------------------------\n");
}

void printVec3(vec3 in) 
{
	printf("(%f, %f, %f)\n", in.x, in.y, in.z);
}

/* Utility functions for easier uploads to shaders with error messages. */
// NEW as prototype 2022, added to VU 2023

#define NUM_ERRORS 8

static void ReportError(const char *caller, const char *name)
{
	static unsigned int draw_error_counter = 0; 
	if(draw_error_counter < NUM_ERRORS)
	{
		fprintf(stderr, "%s warning: '%s' not found in shader!\n", caller, name);
		draw_error_counter++;
	}
	else if(draw_error_counter == NUM_ERRORS)
	{
		fprintf(stderr, "%s: Number of errors bigger than %i. No more vill be printed.\n", caller, NUM_ERRORS);
		draw_error_counter++;
	}
}

void uploadMat4ToShader(GLuint shader, const char *nameInShader, mat4 m)
{
	if (nameInShader == NULL) return;
	glUseProgram(shader);
	GLint loc = glGetUniformLocation(shader, nameInShader);
	if (loc >= 0)
		glUniformMatrix4fv(loc, 1, GL_TRUE, m.m);
	else
		ReportError("uploadMat4ToShader", nameInShader);
}

void uploadUniformIntToShader(GLuint shader, const char *nameInShader, GLint i)
{
	if (nameInShader == NULL) return;
	glUseProgram(shader);
	GLint loc = glGetUniformLocation(shader, nameInShader);
	if (loc >= 0)
		glUniform1i(loc, i);
	else
		ReportError("uploadUniformIntToShader", nameInShader);
}

void uploadUniformFloatToShader(GLuint shader, const char *nameInShader, GLfloat f)
{
	if (nameInShader == NULL) return;
	glUseProgram(shader);
	GLint loc = glGetUniformLocation(shader, nameInShader);
	if (loc >= 0)
		glUniform1f(loc, f);
	else
		ReportError("uploadUniformFloatToShader", nameInShader);
}

void uploadUniformFloatArrayToShader(GLuint shader, const char *nameInShader, GLfloat *f, int arrayLength)
{
	if (nameInShader == NULL) return;
	glUseProgram(shader);
	GLint loc = glGetUniformLocation(shader, nameInShader);
	if (loc >= 0)
		glUniform1fv(loc, arrayLength, f);
	else
		ReportError("uploadUniformFloatToShader", nameInShader);
}

void uploadUniformVec3ToShader(GLuint shader, const char *nameInShader, vec3 v)
{
	if (nameInShader == NULL) return;
	glUseProgram(shader);
	GLint loc = glGetUniformLocation(shader, nameInShader);
	if (loc >= 0)
		glUniform3f(loc, v.x, v.y, v.z);
	else
		ReportError("uploadUniformVec3ToShader", nameInShader);
}

void uploadUniformVec3ArrayToShader(GLuint shader, const char *nameInShader, vec3 *a, int arrayLength)
{
	if (nameInShader == NULL) return;
	glUseProgram(shader);
	GLint loc = glGetUniformLocation(shader, nameInShader);
	if (loc >= 0)
		glUniform3fv(loc, arrayLength, (GLfloat *)a);
	else
		ReportError("uploadUniformVec3ArrayToShader", nameInShader);
}

void bindTextureToTextureUnit(GLuint tex, int unit)
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, tex);
}

#ifdef __cplusplus
mat3 inverse(mat3 m)
{
	return InvertMat3(m);
}
mat4 inverse(mat4 m)
{
	return InvertMat4(m);
}
mat3 transpose(mat3 m)
{
	return TransposeMat3(m);
}
mat4 S(GLfloat s)
{
	return S(s, s, s);
}
mat4 S(vec3 s)
{
	return S(s.x, s.y, s.z);
}
mat4 lookAt(vec3 p, vec3 l, vec3 u)
{
	return lookAtv(p, l, u);
}

mat4 zeroOutTranslation(mat4 lhs) {
	mat4 res {lhs};
	res.m[3] = 0;
	res.m[7] = 0;
	res.m[11] = 0;
	return res;
}

#endif // cplusplus