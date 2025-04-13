#version 330 core

in vec3 eye_position;
in vec3 eye_normal;
in vec4 light_position;

uniform sampler2D shadow_map;
uniform vec3 lightDir;

uniform mat4 view;
uniform mat4 projection;

// Global variables.
vec3 ambientColor = vec3(0.2f);
vec3 diffuseColor = vec3(0.8f);

vec3 CalculateLightDirection(vec3 normal)
{
	vec3 eye_lightDir = (view * vec4(lightDir, 0.0f)).xyz;

	float cosTheta = max(dot(normal, normalize(eye_lightDir)), 0.0f);

    vec3 ambient = ambientColor;
    vec3 diffuse = diffuseColor * cosTheta;

	return ambient + diffuse;
}

float ChebyshevCalculation(float distance, vec2 position)
{
	vec2 moments = texture(shadow_map, position).rg;
	
	if (distance <= moments.x) return 1.0f;
	
	float variance = moments.y - (moments.x * moments.x);
	variance = min(max(variance, 0.00002f), 1.0f);

	float d = distance - moments.x;
	float p_max = variance / (variance + d*d);
	p_max = clamp(p_max, 0, 1);

	return p_max;
}

void main()
{
	vec3 light = CalculateLightDirection(normalize(eye_normal));

	vec4 fragment_position = light_position / light_position.w;
	float shadow = ChebyshevCalculation(fragment_position.z, fragment_position.xy);

	gl_FragColor = vec4(vec3(shadow), 1.0f) * vec4(light, 1.0f);
}
