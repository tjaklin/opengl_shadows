#version 330 core

layout (location=0) in vec3 position;
layout (location=1) in vec3 normal;

uniform mat4 mvp;

out vec4 mvp_position;

void main()
{
    mvp_position = mvp * vec4(position, 1.0f);
    gl_Position = mvp_position;
}
