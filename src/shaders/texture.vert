#version 150

in  vec3 inPosition;
in  vec3 inNormal;
in vec2 inTexCoord;

out vec2 texCoord;
out vec3 lightForwarder;


uniform mat4 projMatrix;
uniform mat4 mdlMatrix;	

const vec3 light = vec3(0.58, 0.58, 0.58);

void main(void)
{
	mat3 normalMatrix1 = mat3(mdlMatrix);
	texCoord = inTexCoord;
	gl_Position = projMatrix * mdlMatrix * vec4(inPosition, 1.0);

	//vec3 translated_n = normalize(mat3(worldToViewMatrix*myMatrix) * in_Normal);	
	float shade=max(dot(normalize(inNormal),light),0.0);
	lightForwarder =vec3(shade,shade,shade);
}
