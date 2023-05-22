#version 150

out vec4 out_Color;
in vec2 texCoord;

uniform sampler2D texUnit;

// NEver ever put lightning for skyboxes !!

void main(void)
{
	out_Color = texture(texUnit, texCoord) * vec4(0.05, 0.458, 0.362, 1.0) ;
    out_Color.b += 0.2 * sin(texCoord.t);
}
