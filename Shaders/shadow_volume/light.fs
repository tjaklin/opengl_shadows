#version 330 core

struct PointLight
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
};

uniform sampler2D texture_position;
uniform sampler2D texture_normal;

uniform PointLight light_source;

vec3 CalculatePointLight(vec3 position, vec3 normal)
{
    vec3 light_direction = normalize(light_source.position - position);

    // Diffuse value
    float cosTheta = max(dot(normal, light_direction), 0.0f);

    // Combine results
    vec3 ambient = light_source.ambient;
    vec3 diffuse = light_source.diffuse * cosTheta;

    return ambient + diffuse;
}

void main(void)
{
	const vec2 screen_size = vec2(1024, 1024);
   	vec2 texture_uv = gl_FragCoord.xy / screen_size;

   	vec3 position_sampled = texture(texture_position, texture_uv).xyz;
   	vec3 normal_sampled = normalize(texture(texture_normal, texture_uv).xyz);

	vec3 light = CalculatePointLight(position_sampled, normal_sampled);
   	gl_FragColor = vec4(light, 1.0f);
}
