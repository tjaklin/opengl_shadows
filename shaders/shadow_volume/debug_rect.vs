#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 uv;

out vec2 textureUv;

void main()
{
    textureUv = uv.xy;
    gl_Position = vec4(position, 1.0f);
}