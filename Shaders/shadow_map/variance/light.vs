#version 330 core

layout (location=0) in vec3 position;
layout (location=1) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 biasMVP;

out vec3 eye_position;
out vec3 eye_normal;
out vec4 light_position;

void main()
{
	eye_position = (view * model * vec4(position, 1.0f)).xyz;
	eye_normal = (view * model * vec4(normal, 0.0f)).xyz;
	light_position = biasMVP * vec4(position, 1.0f);

	gl_Position = projection * view * model * vec4(position, 1.0f);
}
