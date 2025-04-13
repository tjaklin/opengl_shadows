#version 330 core

in vec4 mvp_position;

void main()
{
	float depth = mvp_position.z / mvp_position.w;
	depth = depth * 0.5f + 0.5f;

	float dx = dFdx(depth);
	float dy = dFdy(depth);

	float moment1 = depth;
	float moment2 = depth * depth + (0.25f * (dx*dx + dy*dy));

	gl_FragColor = vec4(moment1, moment2, 0.0f, 0.0f);
}