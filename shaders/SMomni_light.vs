#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normals;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform bool reverse_normals;

out vec3 world_position;
out vec3 eye_position;
out vec3 eye_normals;

void main()
{
	vec4 pos = vec4( position, 1.0f );
	vec4 nor = vec4( normals,  0.0f );

	world_position = (model * pos).xyz;
	eye_position   = (view * model * pos).xyz;

    if(reverse_normals)
	eye_normals =(view * model * -nor ).xyz;
    else
	eye_normals =(view * model *  nor ).xyz;

	gl_Position = projection * view *  model * pos;
}