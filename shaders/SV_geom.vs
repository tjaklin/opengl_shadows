#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normals;

uniform mat4 MVP;
uniform mat4 model;
uniform bool reverse_normals;

out vec3 wsPos;
out vec3 wsNor;

void main(){

	gl_Position = MVP * vec4( position, 1.0);

	wsPos = ( model * vec4( position, 1.0) ).xyz;

	if(reverse_normals)
		wsNor = ( model * vec4(-normals, 0.0) ).xyz;
	else
		wsNor = ( model * vec4( normals, 0.0) ).xyz;
}