#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 uv;

out vec2 texture_uv;

void main()
{
    texture_uv = uv.xy;
    gl_Position = vec4(position, 1.0f);
}
