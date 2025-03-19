#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;    // Change to normal sometime later...
// layout (location = 2) in vec2 textureUV;

out vec4 in_color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
    in_color = vec4(color, 1.0f);
}
