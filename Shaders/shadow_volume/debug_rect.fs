#version 330 core

in vec2 textureUv;

out vec4 FragColor;

uniform sampler2D depthMap;

void main()
{             
    float depthValue = texture(depthMap, textureUv).r;
    FragColor = vec4(vec3(depthValue), 1.0); // orthographic
}
