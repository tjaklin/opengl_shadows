#version 330 core

in vec3 eye_position;
in vec3 eye_normals;
in vec4 light_position;

uniform vec3 lightDir;
uniform sampler2D shadowMap;
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

vec3 CalculateShadow(void)
{
	// Perform perspective divide
	vec3 l_pos = light_position.xyz / light_position.w;
	// Transform 'l_pos' to [0,1] range
	//	Disable this calculation when using a 'biasMatrix'.
	l_pos = l_pos * 0.5 + 0.5;

	const float bias = 0.0005f;
	float currentDepth = l_pos.z - bias;
	float closestDepth = texture(shadowMap, l_pos.xy).r;
	
	if (currentDepth > closestDepth)
		return vec3(0.4f);	// Fragment in shadow.
	else
		return vec3(1.0f);	// Fragment not in shadow.
}

void main()
{
	vec3 light = CalculateDirectionalLight(normalize(eye_normals));
	vec3 shadow = CalculateShadow();

	gl_FragColor = vec4(shadow, 1.0f ) * vec4(light, 1.0f);
}
