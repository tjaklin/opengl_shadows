#version 330 core

out vec4 FragColor;

void main(void)
{
    vec3 color = vec3(0.4f, 0.9f, 0.4f);
    FragColor = vec4(color, 0.5f);
}
