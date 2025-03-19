#version 330 core

in vec4 in_color;
out vec4 color;

void main()
{
    color = vec4(0.7f, 0.5f, 0.75f, 1.0f);
    color = in_color;
}
