#version 330 core

layout (location=0) in vec3 position;

uniform mat4 model;

out vec3 world_position;

void main(void)
{	
    world_position = (model * vec4(position, 1.0f)).xyz;
}
