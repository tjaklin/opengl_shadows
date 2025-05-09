#version 330 core

uniform vec2 scale_factor;
uniform sampler2D shadow_map;

in vec2 texture_coordinate;

void main()
{
	vec4 color = vec4(0.0);
	color += texture2D(shadow_map, texture_coordinate + vec2(-3.0 * scale_factor.x, -3.0 * scale_factor.y))* 1/64;
	color += texture2D(shadow_map, texture_coordinate + vec2(-2.0 * scale_factor.x, -2.0 * scale_factor.y))* 6/64;
	color += texture2D(shadow_map, texture_coordinate + vec2(-1.0 * scale_factor.x, -1.0 * scale_factor.y))* 15/64;
	color += texture2D(shadow_map, texture_coordinate + vec2(0.0, 0.0)) * 20/64;
	color += texture2D(shadow_map, texture_coordinate + vec2(1.0 * scale_factor.x,  1.0 * scale_factor.y)) * 15/64;
	color += texture2D(shadow_map, texture_coordinate + vec2(2.0 * scale_factor.x,  2.0 * scale_factor.y)) * 6/64;
	color += texture2D(shadow_map, texture_coordinate + vec2(3.0 * scale_factor.x, -3.0 * scale_factor.y)) * 1/64;

	gl_FragColor = color;
}
