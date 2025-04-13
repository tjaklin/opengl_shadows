#version 330 core

in vec3 world_position;
in vec3 eye_position;
in vec3 eye_normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 light_position;
uniform float far_plane;

uniform samplerCube shadow_map;

// Global variables.
vec3 ambientColor = vec3(0.2f);
vec3 diffuseColor = vec3(0.8f);

vec3 CalculatePointLight(vec3 normal)
{
    vec3 eye_lightPos = (view * vec4(light_position, 1.0f)).xyz;

    vec3 light_direction = normalize(eye_lightPos - eye_position);

    float cosTheta = max(dot(normal, light_direction), 0.0);

	// Diffuse and ambient color values.
    vec3 ambient = ambientColor;
    vec3 diffuse = diffuseColor * cosTheta;

return ambient + diffuse;
}

float CalculateShadow(bool use_pcf_correction)
{
	float bias = 0.05f;

	if (!use_pcf_correction)
	{
		vec3 fragment_to_light = world_position - light_position;
		float current_depth = length(fragment_to_light);

		float closest_depth = texture(shadow_map, fragment_to_light).r;
		closest_depth = closest_depth * far_plane;

		float shadow_value = (current_depth - bias) > closest_depth ? 0.4f : 1.0f;
		return shadow_value;
	}
	else
	{
		vec3 fragment_to_light = world_position - light_position;
		float current_depth = length(fragment_to_light);

		// Get the 'closest_depth' value based on a few neighbouring fragments
		//	and modify the 'shadow_value' value accordingly.
		float shadow_value_sum = 0.0f;
		float samples = 4.0f;
		float offset = 0.1f;
		for(float x = -offset; x < offset; x += offset / (samples * 0.5f))
	    	for(float y = -offset; y < offset; y += offset / (samples * 0.5f))
				for(float z = -offset; z < offset; z += offset / (samples * 0.5f))
				{
					float closest_depth = texture(shadow_map, fragment_to_light + vec3(x, y, z)).r;
					closest_depth = closest_depth * far_plane;

					float shadow_value = (current_depth - bias) > closest_depth ? 0.4f : 0.0f;
					shadow_value_sum += shadow_value;
				}

		shadow_value_sum = shadow_value_sum / (samples * samples * samples);
		shadow_value_sum = 1 - shadow_value_sum;
		return shadow_value_sum;
	}
}

void main()
{
	// Fragment's 'lighting' value
	vec3 lightingValue = CalculatePointLight(normalize(eye_normal));

	// Fragment's 'shadowing' value
	bool use_pcf_correction = true;
	float shadowingValue = CalculateShadow(use_pcf_correction);

	// Combine the values of 'lighting' and 'shadowing'
	//	to get the final result color.
	gl_FragColor = vec4((lightingValue * shadowingValue), 1.0);
}