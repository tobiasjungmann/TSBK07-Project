#version 150

in  vec3 in_Position;
in 	vec3 in_Normal;
in vec2 inTexCoord;


out vec3 normalInViewCoordinates;
out vec3 posInViewCoordinates;
out vec2 texCoord;

uniform mat4 w2vMatrix;
uniform mat4 m2wMatrix;
uniform mat4 projectionMatrix;


void main(void)
{
	vec4 posInViewVec4 = w2vMatrix * m2wMatrix * vec4(in_Position, 1.0);
	posInViewCoordinates = vec3(posInViewVec4);
	normalInViewCoordinates = mat3(w2vMatrix * m2wMatrix) * in_Normal;
	gl_Position = projectionMatrix * posInViewVec4;
    texCoord = inTexCoord;
}
