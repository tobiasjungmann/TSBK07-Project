#include "VectorUtils4.h"

#ifdef __cplusplus
	//	vec3::vec3(vec4 v) : x(v.x), y(v.y), z(v.z) {}

	constexpr vec2::vec2() : x{}, y{} {}
	constexpr vec2::vec2(GLfloat x2, GLfloat y2) : x{x2}, y{y2} {}

	constexpr vec3::vec3() : x{}, y{}, z{} {}
	constexpr vec3::vec3(GLfloat x2, GLfloat y2, GLfloat z2) : x{x2}, y{y2}, z{z2} {}
	constexpr vec3::vec3(GLfloat x2) : x{x2}, y{x2}, z{x2} {}
	constexpr vec3::vec3(vec4 v) : x{v.x}, y{v.y}, z{v.z} {}

	constexpr vec4::vec4() : x{}, y{}, z{}, h{} {}
	constexpr vec4::vec4(GLfloat x2, GLfloat y2, GLfloat z2, GLfloat w2) : x{x2}, y{y2}, z{z2}, w{w2} {}
	constexpr vec4::vec4(GLfloat xyz, GLfloat w2) : x{xyz}, y{xyz}, z{xyz}, w{w2} {}
	constexpr vec4::vec4(vec3 v, GLfloat w2) : x{v.x}, y{v.y}, z{v.z}, w{w2} {}
	constexpr vec4::vec4(vec3 v) : x{v.x}, y{v.y}, z{v.z}, w{1} {}

 	constexpr mat4::mat4() : m{} {}
	constexpr mat4::mat4(GLfloat x2) :
	m{ x2, 0, 0, 0, 
		0, x2, 0, 0,
		0, 0, x2, 0,
		0, 0, 0, x2} {}

	constexpr mat4::mat4(GLfloat p0, GLfloat p1, GLfloat p2, GLfloat p3,
		GLfloat p4, GLfloat p5, GLfloat p6, GLfloat p7,
		GLfloat p8, GLfloat p9, GLfloat p10, GLfloat p11, 
		GLfloat p12, GLfloat p13, GLfloat p14, GLfloat p15) :
	m {p0, p1, p2, p3,
		p4, p5, p6, p7,
		p8, p9, p10, p11,
		p12, p13, p14, p15} {}
	constexpr mat4::mat4(mat3 x) : 
	m{ 	x.m[0],  x.m[1],  x.m[2],  0,
		x.m[3],  x.m[4],  x.m[5],  0,
		x.m[6],  x.m[7],  x.m[8],  0,
		x.m[0],  x.m[0],  x.m[0],  1} {}

  inline void mat4::setLine(unsigned char line, vec4 lineValue) noexcept
  {
    m[0 + (line << 2)] = lineValue.x;
    m[1 + (line << 2)] = lineValue.y;
    m[2 + (line << 2)] = lineValue.z;
    m[3 + (line << 2)] = lineValue.a;
  }

  constexpr mat3::mat3() : m{} {}
	constexpr mat3::mat3(GLfloat x2) :
	m{x2, 0, 0, 0, x2, 0, 0, 0, x2}
	{}
	constexpr mat3::mat3(GLfloat p0, GLfloat p1, GLfloat p2,
		GLfloat p3, GLfloat p4, GLfloat p5,
		GLfloat p6, GLfloat p7, GLfloat p8) :
		m{p0, p1, p2, p3, p4, p5, p6, p7, p8}
	{}
	constexpr mat3::mat3(mat4 x) :
	m{x.m[0], x.m[1], x.m[2],
	x.m[4], x.m[5], x.m[6],
	x.m[8], x.m[9], x.m[10]}
	{}

	constexpr mat3::mat3(vec3 x1, vec3 x2, vec3 x3) :
	m{x1.x, x1.y, x1.z, x2.x, x2.y, x2.z, x3.x, x3.y, x3.z}
	{}


  inline void mat3::setLine(unsigned char line, vec3 lineValue) noexcept
  {
    m[0 + line * 3] = lineValue.x;
    m[1 + line * 3] = lineValue.y;
    m[2 + line * 3] = lineValue.z;
  }
#endif // __cplusplus
