#version 330 core

layout (location=0) in vec3 position;
layout (location=1) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lBiasMVP;

out vec3 eye_position;
out vec3 eye_normals;
out vec4 light_position;

void main()
{

	vec4 pos = vec4( position, 1.0f );
	vec4 nor = vec4( normal , 0.0f );

	light_position = lBiasMVP * pos;
	eye_position   = ( view * model * pos).xyz;
	eye_normals    = ( view * model * nor).xyz;

	gl_Position = projection * view * model * pos;
}