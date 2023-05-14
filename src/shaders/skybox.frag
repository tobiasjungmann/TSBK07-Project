#version 150

out vec4 out_Color;
in vec2 texCoord;

uniform sampler2D texUnit;

// NEver ever put lightning for skyboxes !!

void main(void)
{
	out_Color = texture(texUnit, texCoord); 
}
