#version 330 core

layout (location=0) in vec3 position;
layout (location=1) in vec3 uv;

uniform mat4 model;

out vec4 world_uv;

void main(void)
{
    world_uv = vec4(uv, 1.0f);
    gl_Position = model * vec4(position, 1.0f);
}
