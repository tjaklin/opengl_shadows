#version 330 core

in vec2 texture_uv;

out vec4 FragColor;

uniform sampler2D depth_map;

void main()
{             
    float depth_value = texture(depth_map, texture_uv).r;
    FragColor = vec4(vec3(depth_value), 1.0); // orthographic
}
