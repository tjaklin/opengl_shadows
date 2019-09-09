#version 330 core

in vec3 wsPos;
in vec3 wsNor;

uniform sampler2D myTex0;

layout (location = 0) out vec4 oPos;
layout (location = 1) out vec4 oNor;

void main(){

	oPos = vec4( wsPos, 1.0);
	oNor = normalize( vec4( wsNor, 1.0) );
}