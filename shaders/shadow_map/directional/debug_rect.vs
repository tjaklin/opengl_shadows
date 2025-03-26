#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 uv;

out vec2 textureUv;
out vec4 color;

void main()
{
    textureUv = uv.xy;
    color = vec4(position, 1.0f);
    gl_Position = vec4(position, 1.0f);
}