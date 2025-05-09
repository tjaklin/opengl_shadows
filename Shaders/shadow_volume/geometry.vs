#version 330 core

layout (location=0) in vec3 position;
layout (location=1) in vec3 normal;

uniform mat4 MVP;
uniform mat4 model;

out vec3 world_position;
out vec3 world_normal;

void main(void)
{
	world_position = (model * vec4(position, 1.0f)).xyz;
	world_normal = (model * vec4(normal, 0.0f)).xyz;

	gl_Position = MVP * vec4(position, 1.0f);
}
