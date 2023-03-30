#version 150

out vec4 outColor;
in vec2 texCoord;
uniform sampler2D tex;
in vec3 lightForwarder;
void main(void)
{
	outColor = vec4(1.0,0.0,0.0,0.5);//texture(tex, texCoord)*vec4(lightForwarder,1.0);
}
