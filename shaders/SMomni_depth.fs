#version 330 core

in vec4 screen_pos;

uniform vec3 lightPos;
uniform float far_plane;

void main()
{
	float distance = length( screen_pos.xyz - lightPos );

	distance = distance / far_plane;

	gl_FragDepth = distance;
}