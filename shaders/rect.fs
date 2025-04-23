#version 330 core

in vec4 world_uv;

uniform sampler2D texture_image;

out vec4 FragColor;

void main(void)
{
    vec3 color = vec3(texture(texture_image, world_uv.xy));
    FragColor = vec4(color, 0.5f);
}
