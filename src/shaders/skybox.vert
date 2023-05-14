#version 150

in  vec3 in_Position;
in vec2 inTexCoord;

out vec2 texCoord;

uniform mat4 preProjTransform;
uniform mat4 projectionMatrix;

void main(void)
{
	gl_Position = projectionMatrix * preProjTransform * vec4(in_Position, 1.0);
	texCoord = inTexCoord;
}
