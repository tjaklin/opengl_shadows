#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform mat4 lightMVP;

void main()
{
	gl_Position = lightMVP * vec4(position, 1.0f);
}