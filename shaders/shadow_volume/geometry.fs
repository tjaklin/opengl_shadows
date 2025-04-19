#version 330 core

in vec3 world_position;
in vec3 world_normal;

uniform sampler2D texture_zero;	// NOTE: Not used her; not set in the main program.

layout (location=0) out vec4 out_position;
layout (location=1) out vec4 out_normal;
// We choose not to write to the texture bound to GL_COLOR_ATTACHMENT2.
// We also write to the Depth texture bound to the active FBO object.

// DEBUG:
out vec4 color;

void main(void)
{
	// NOTE: Commented out for debugging purposes. Uncomment later.
	//out_position = vec4(world_position, 1.0f);
	//out_normal = normalize(vec4(world_normal, 1.0f));
	//color = vec4(world_position.x, world_position.y, world_position.z, 1.0f);
	color = vec4(world_normal, 1.0f);
}