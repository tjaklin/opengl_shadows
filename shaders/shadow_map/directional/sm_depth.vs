#version 330 core

layout (location = 0) in vec3 position;

uniform mat4 lMVP;

void main()
{
	gl_Position = lMVP * vec4(position, 1.0f);
}