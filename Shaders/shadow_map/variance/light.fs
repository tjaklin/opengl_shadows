#version 330 core

in vec3 eye_position;
in vec3 eye_normal;
in vec4 light_position;

uniform vec3 lightDir;
uniform sampler2D shadow_map;
uniform mat4 view;
uniform mat4 projection;

// Global variables.
vec3 ambientColor = vec3(0.2f);
vec3 diffuseColor = vec3(0.8f);

vec3 CalculateDirectionalLight(vec3 normal)
{
	vec3 eye_lightPos = (view * vec4(lightDir, 0.0f)).xyz;
	
	vec3 light_direction = normalize(eye_lightPos);

	float cosTheta = max(dot(normal, light_direction), 0.0f);

	// Diffuse and ambient color values.
	vec3 ambient = ambientColor;
	vec3 diffuse = diffuseColor * cosTheta;

	return ambient + diffuse;
}

float ChebyshevCalculation(void)
{
	// Perform perspective divide
	vec3 l_pos = light_position.xyz / light_position.w;
	// Transform 'l_pos' to [0,1] range
	//	Disable this calculation when using a 'biasMatrix'.
	//l_pos = l_pos * 0.5 + 0.5;

	const float bias = 0.0005f;
	float currentDepth = l_pos.z - bias;
	vec2 moments = texture(shadow_map, l_pos.xy).rg;
	
	if (currentDepth <= moments.x) return 1.0f;	// Not in shadow.

	// Variance technique's specific calculations.
	float variance = moments.y - (moments.x * moments.x);
	variance = min(max(variance, 0.00002f), 1.0f);

	float d = currentDepth - moments.x;
	float p_max = variance / (variance + d*d);
	p_max = clamp(p_max, 0, 1);

	return p_max;
}

void main()
{
	vec3 light = CalculateDirectionalLight(normalize(eye_normal));
	float shadow = ChebyshevCalculation();

	gl_FragColor = vec4(vec3(shadow), 1.0f) * vec4(light, 1.0f);
}
