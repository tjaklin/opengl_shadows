#version 330 core

in vec4 screen_pos;

uniform vec3 light_position;
uniform float far_plane;

void main()
{
	float distance = length(screen_pos.xyz - light_position);

	distance = distance / far_plane;

	gl_FragDepth = distance;
}